// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <xme/xmecallback.h>
#include <xme/xmecontrol.h>
#include <xme/xmelicense.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include <diagnostic_updater/diagnostic_updater.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <lifecycle_msgs/msg/state.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <rclcpp_lifecycle/lifecycle_publisher.hpp>
#include <tf2_ros/transform_broadcaster.hpp>
#include <xsens_mvn_ros2_common/body_dimensions_parameters.hpp>
#include <xsens_mvn_ros2_common/skeleton_publisher.hpp>
#include <xsens_mvn_ros2_xme/xsens_xme_node_parameters.hpp>

#if __has_include(<magic_enum/magic_enum.hpp>)
#include <magic_enum/magic_enum.hpp>
#else
#include <magic_enum.hpp>
#endif

#include "rclcpp_action/rclcpp_action.hpp"
#include "xsens_mvn_msgs/action/perform_calibration.hpp"
#include "xsens_mvn_msgs/srv/load_calibration_path.hpp"

namespace xsens_xme_ros2
{

template<typename Enum>
std::string getEnumName(Enum value)
{
  return std::string(magic_enum::enum_name(value));
}

enum class ConfigurationResult
{
  FAILED,
  SUCCESS
};

enum class ConfigurationProcessStep
{
  NOT_STARTED,
  IN_PROGRESS,
  COMPLETED
};

struct ConfigurationChangeStatus
{
  std::atomic<ConfigurationProcessStep> processStatus = ConfigurationProcessStep::NOT_STARTED;
  std::atomic<ConfigurationResult> result = ConfigurationResult::FAILED;

  void reset()
  {
    processStatus = ConfigurationProcessStep::NOT_STARTED;
    result = ConfigurationResult::FAILED;
  }
};

class XsensXmeNode : public rclcpp_lifecycle::LifecycleNode, public XmeCallback
{
public:
  XsensXmeNode();
  ~XsensXmeNode() final;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_configure(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_activate(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_cleanup(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_shutdown(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_error(const rclcpp_lifecycle::State &) override;

protected:
  void onPoseReady(XmeControl *) override;
  void onHardwareReady(XmeControl *) override;
  void onHardwareError(XmeControl *) override;
  void onHardwareDisconnected(XmeControl *) override;
  // For calibration
  void onCalibrationRecordingSectionChanged(
    XmeControl * dev, XmeCalibrationRecordingSection calibSection) override;
  void onCalibrationAborted(XmeControl *) override;
  void onCalibrationComplete(XmeControl *) override;
  void onCalibrationProcessed(XmeControl *) override;
  void onProgressUpdate(XmeControl * dev, int percentage, XsString const * category) override;
  void onProcessingProgress(
    XmeControl * dev, XmeProcessingStage stage, int firstFrame, int lastFrame) override;
  // For configuration changes
  void onConfigurationChangeComplete(XmeControl *) override;
  void onConfigurationChangeFailed(XmeControl * xme) override;

private:
  using PerformCalibration = xsens_mvn_msgs::action::PerformCalibration;
  using GoalHandlePerformCalibration = rclcpp_action::ServerGoalHandle<PerformCalibration>;
  // Parameters
  std::shared_ptr<xsens_xme_node::ParamListener> m_paramListenerXme;
  std::shared_ptr<body_dimensions::ParamListener> m_paramListenerBody;
  // Xsens related
  std::optional<XmeLicense> m_license;
  XmeControl * m_xmeControl = nullptr;
  // Hardware error tracking
  mutable std::mutex m_hardwareErrorMutex;
  XsString m_lastError;
  XsIntArray m_missingTrackers;
  XsIntArray m_duplicateTrackers;
  int m_awindaChannel = 0;
  // Calibration related
  std::atomic<bool> m_calibrationValid{true};
  std::atomic<bool> m_errorReceived = false;
  std::atomic<bool> m_calibrationProcessed = false;
  std::atomic<bool> m_calibrationComplete = false;
  XmeCalibrationRecordingSection m_calibSection;
  std::shared_ptr<GoalHandlePerformCalibration> m_activeGoalHandle;
  std::thread m_calibrationThread;
  void setCalibrationpath(
    const xsens_mvn_msgs::srv::LoadCalibrationPath::Request::SharedPtr request,
    xsens_mvn_msgs::srv::LoadCalibrationPath::Response::SharedPtr response);
  bool waitForRecordingState() const;
  std::stringstream getCalibrationResults(const XmeCalibrationResult & calibrationResult) const;
  void logAndPublishFeedback(const std::string & statusMsg) const;
  bool checkIfGoalCanceled();
  void abortActionGoal(const std::string & calibrationResult);
  // XME Configuration
  ConfigurationChangeStatus m_configChangeStatus;
  bool setSampleRate(double rate);
  bool setSuitConfiguration(const std::string & configName);
  void setDimension(const std::string & label, double value);
  bool setBiomechanicalModel(const std::string & biomechModel);

  // ROS2 related
  std::unique_ptr<tf2_ros::TransformBroadcaster> m_tfBroadcaster;
  std::unique_ptr<xsens_mvn_ros2::SkeletonPublisher> m_skeletonPub;
  std::unique_ptr<diagnostic_updater::Updater> m_diagnosticUpdater;
  void diagnosticsCallback(diagnostic_updater::DiagnosticStatusWrapper & stat);
  rclcpp::TimerBase::SharedPtr m_diagnosticTimer;
  rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Point>::SharedPtr m_comPublisher;
  rclcpp::Service<xsens_mvn_msgs::srv::LoadCalibrationPath>::SharedPtr m_setCalibrationPathService;
  rclcpp_action::Server<PerformCalibration>::SharedPtr m_performCalibrationActionServer;

  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr m_onSetParameterCallback;
  rcl_interfaces::msg::SetParametersResult onSetParametersCallback(
    const std::vector<rclcpp::Parameter> & parameters);

  // action server for calibration
  rclcpp_action::GoalResponse handleGoal(
    const rclcpp_action::GoalUUID & uuid,
    std::shared_ptr<const PerformCalibration::Goal> goal);
  rclcpp_action::CancelResponse handleCancel(
    const std::shared_ptr<GoalHandlePerformCalibration> goalHandle) const;
  void handleAccepted(const std::shared_ptr<GoalHandlePerformCalibration> goalHandle);
  void execute(const std::shared_ptr<GoalHandlePerformCalibration> goalHandle);
};
}  // namespace xsens_xme_ros2
