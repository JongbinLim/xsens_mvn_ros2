// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include "xsens_mvn_ros2_xme/xsens_xme_node.hpp"

#include <xme/xmedll.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <tf2_eigen/tf2_eigen.hpp>
#include <xsens_mvn_ros2_common/xsens_model.hpp>

constexpr auto NODE_NAME = "xsens_mvn_ros2_xme_node";

namespace xsens_xme_ros2
{

struct calibration_status
{
  bool error_received = false;
  bool calibration_processed;
  int total_sections;
};

XsensXmeNode::XsensXmeNode()
: rclcpp_lifecycle::LifecycleNode(NODE_NAME)
{
}

XsensXmeNode::~XsensXmeNode()
{
  // Failsafe cleanup in case lifecycle shutdown was not called
  if (m_xmeControl) {
    m_xmeControl->removeCallbackHandler(this);
    m_xmeControl->setScanMode(false);
    m_xmeControl->disconnectHardware();
    m_xmeControl->destruct();
    m_xmeControl = nullptr;
    xmeTerminate();
  }
  RCLCPP_INFO(get_logger(), "Destroyed.");
}

void disconnectHardwareWithTimeout(
  XmeControl * xme, rclcpp::Logger logger, rclcpp::Clock::SharedPtr clock)
{
  xme->setScanMode(false);
  xme->disconnectHardware();
  auto dc_start = std::chrono::steady_clock::now();
  while (xme->status().isConnected()) {
    if (std::chrono::steady_clock::now() - dc_start > std::chrono::seconds(10)) {
      RCLCPP_WARN(
        logger,
        "Hardware disconnect timeout after 10 seconds, proceeding.");
      break;
    }
    RCLCPP_INFO_THROTTLE(
      logger, *clock, 1000,
      "Waiting for hardware disconnection...");
  }
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensXmeNode::on_configure(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(get_logger(), "Configuring Xsens XME Node...");

  // Declare parameters with type-safe bounds validation via generate_parameter_library.
  m_paramListenerXme = std::make_shared<xsens_xme_node::ParamListener>(
    get_node_parameters_interface());
  m_paramListenerBody = std::make_shared<body_dimensions::ParamListener>(
    get_node_parameters_interface());
  const auto params = m_paramListenerXme->get_params();

  // Set SDK path for related files to be found
  XsString settingsPath(params.settingsPath.c_str());
  XsString userPath(params.userPath.c_str());
  XsString logPath(params.logPath.c_str());
  RCLCPP_INFO_STREAM(
    get_logger(), "Using settings path: " << settingsPath.c_str() << ", user path: "
                                          << userPath.c_str() << ", log path: " << logPath.c_str());
  xmeSetPaths(settingsPath, userPath, logPath);

  // Let's check for license first
  m_license.emplace();
  if (!XmeLicense::isConstructed()) {
    RCLCPP_ERROR(get_logger(), "License system not constructed");
    m_license.reset();
    m_paramListenerXme.reset();
    m_paramListenerBody.reset();
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::FAILURE;
  }

  RCLCPP_INFO_STREAM(
    get_logger(), "XME Version: " << xmeGetDllVersion().toString().c_str()
                                  << " License: " << m_license->getCurrentLicense().c_str());

  m_awindaChannel = params.awindaChannel;
  RCLCPP_INFO(
    get_logger(), "Using Awinda radio channel %d (range 11-25)", m_awindaChannel);

  // Initialize the xme control
  m_xmeControl = XmeControl::construct();

  if (!m_xmeControl) {
    RCLCPP_ERROR_STREAM(
      get_logger(),
      "Unable to create XmeControl: " << XsResultValue_toString(XmeControl::creationResultCode()));
    m_license.reset();
    m_paramListenerXme.reset();
    m_paramListenerBody.reset();
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::FAILURE;
  }
  m_xmeControl->addCallbackHandler(this);
  m_xmeControl->setRealTimePoseMode(true);

  // Register parameter change callback after all params are declared.
  m_onSetParameterCallback = add_on_set_parameters_callback(
    std::bind(&XsensXmeNode::onSetParametersCallback, this, std::placeholders::_1));

  // Starting scan
  m_xmeControl->setScanMode(true);

  {
    auto hw_start = std::chrono::steady_clock::now();
    while (!m_xmeControl->status().isConnected()) {
      auto elapsed = std::chrono::steady_clock::now() - hw_start;
      if (elapsed > std::chrono::seconds(30)) {
        RCLCPP_ERROR(
          get_logger(),
          "Hardware connection timeout after 30 seconds. "
          "Check: (1) Is the hardware powered on? "
          "(2) Is the USB dongle connected? "
          "(3) Is the correct Awinda channel configured?");
        m_xmeControl->removeCallbackHandler(this);
        m_xmeControl->setScanMode(false);
        m_xmeControl->destruct();
        m_xmeControl = nullptr;
        m_license.reset();
        m_onSetParameterCallback.reset();
        m_paramListenerXme.reset();
        m_paramListenerBody.reset();
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::FAILURE;
      }
      RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 1000, "Waiting for hardware connection...");
    }
  }

  // ROS2 publishers and TF
  m_comPublisher = create_publisher<geometry_msgs::msg::Point>("com", rclcpp::SystemDefaultsQoS());
  m_tfBroadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
  m_skeletonPub = std::make_unique<xsens_mvn_ros2::SkeletonPublisher>(*this, *m_tfBroadcaster);

  // Diagnostics
  m_diagnosticUpdater = std::make_unique<diagnostic_updater::Updater>(this);
  m_diagnosticUpdater->setHardwareID("Xsens MVN Suit");
  m_diagnosticUpdater->add("Suit status", this, &XsensXmeNode::diagnosticsCallback);
  m_diagnosticTimer = create_wall_timer(
    std::chrono::seconds(1), [this]() {m_diagnosticUpdater->force_update();});

  // Services
  m_setCalibrationPathService = create_service<xsens_mvn_msgs::srv::LoadCalibrationPath>(
    "~/set_calibration_path",
    [this](
      const xsens_mvn_msgs::srv::LoadCalibrationPath::Request::SharedPtr request,
      xsens_mvn_msgs::srv::LoadCalibrationPath::Response::SharedPtr response) {
      if (get_current_state().id() !=
      lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
      {
        response->success = false;
        response->message = "Node is not active.";
        return;
      }
      setCalibrationpath(request, response);
    });

  // Actions
  m_performCalibrationActionServer =
    rclcpp_action::create_server<xsens_mvn_msgs::action::PerformCalibration>(
      this, "~/perform_calibration",
      std::bind(&XsensXmeNode::handleGoal, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&XsensXmeNode::handleCancel, this, std::placeholders::_1),
      std::bind(&XsensXmeNode::handleAccepted, this, std::placeholders::_1));

  RCLCPP_INFO(get_logger(), "Configured. Waiting for activation...");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensXmeNode::on_activate(const rclcpp_lifecycle::State &)
{
  m_comPublisher->on_activate();
  m_skeletonPub->activate();

  RCLCPP_INFO(get_logger(), "Activated.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensXmeNode::on_deactivate(const rclcpp_lifecycle::State &)
{
  // Abort in-progress calibration
  if (m_calibrationThread.joinable()) {
    if (m_activeGoalHandle) {
      m_activeGoalHandle.reset();
    }
    m_calibrationThread.join();
  }
  m_comPublisher->on_deactivate();
  m_skeletonPub->deactivate();

  RCLCPP_INFO(get_logger(), "Deactivated.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensXmeNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(get_logger(), "Cleaning up...");

  if (m_calibrationThread.joinable()) {
    m_calibrationThread.join();
  }

  // Remove callback handler FIRST to prevent use-after-free from SDK thread
  if (m_xmeControl) {
    m_xmeControl->removeCallbackHandler(this);
    disconnectHardwareWithTimeout(m_xmeControl, get_logger(), get_clock());
    m_xmeControl->destruct();
    m_xmeControl = nullptr;
  }
  if (m_license.has_value()) {
    m_license.reset();
  }

  // Reset ROS objects
  m_diagnosticTimer.reset();
  m_diagnosticUpdater.reset();
  m_performCalibrationActionServer.reset();
  m_setCalibrationPathService.reset();
  m_onSetParameterCallback.reset();
  m_skeletonPub.reset();
  m_tfBroadcaster.reset();
  m_comPublisher.reset();
  m_paramListenerXme.reset();
  m_paramListenerBody.reset();

  RCLCPP_INFO(get_logger(), "Cleaned up.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensXmeNode::on_shutdown(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(get_logger(), "Shutting down...");

  if (m_calibrationThread.joinable()) {
    m_calibrationThread.join();
  }

  if (m_xmeControl) {
    m_xmeControl->removeCallbackHandler(this);
    disconnectHardwareWithTimeout(m_xmeControl, get_logger(), get_clock());
    m_xmeControl->destruct();
    m_xmeControl = nullptr;
  }
  if (m_license.has_value()) {
    m_license.reset();
  }
  xmeTerminate();

  m_diagnosticTimer.reset();
  m_diagnosticUpdater.reset();
  m_performCalibrationActionServer.reset();
  m_setCalibrationPathService.reset();
  m_onSetParameterCallback.reset();
  m_skeletonPub.reset();
  m_tfBroadcaster.reset();
  m_comPublisher.reset();
  m_paramListenerXme.reset();
  m_paramListenerBody.reset();

  RCLCPP_INFO(get_logger(), "Shut down complete.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensXmeNode::on_error(const rclcpp_lifecycle::State &)
{
  RCLCPP_ERROR(get_logger(), "Error encountered. Cleaning up...");

  if (m_calibrationThread.joinable()) {
    m_calibrationThread.join();
  }

  if (m_xmeControl) {
    m_xmeControl->removeCallbackHandler(this);
    m_xmeControl->setScanMode(false);
    m_xmeControl->disconnectHardware();
    m_xmeControl->destruct();
    m_xmeControl = nullptr;
  }
  if (m_license.has_value()) {
    m_license.reset();
  }

  m_diagnosticTimer.reset();
  m_diagnosticUpdater.reset();
  m_performCalibrationActionServer.reset();
  m_setCalibrationPathService.reset();
  m_onSetParameterCallback.reset();
  m_skeletonPub.reset();
  m_tfBroadcaster.reset();
  m_comPublisher.reset();
  m_paramListenerXme.reset();
  m_paramListenerBody.reset();

  RCLCPP_ERROR(get_logger(), "Error recovery complete, returning to unconfigured.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

void XsensXmeNode::onPoseReady(XmeControl * xme)
{
  if (get_current_state().id() != lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE) {
    return;
  }
  if (!m_skeletonPub) {
    return;
  }
  if (!m_calibrationValid) {
    RCLCPP_WARN_THROTTLE(
      get_logger(), *get_clock(), 5000,
      "Pose data suppressed: body dimensions changed since last calibration. "
      "Perform a new calibration or load a calibration file to resume publishing.");
    return;
  }

  auto pose = xme->pose(XME_LAST_AVAILABLE_FRAME);

  RCLCPP_DEBUG_STREAM(get_logger(), "=== Pose nr " << pose.m_frameNumber << " ====");
  auto stamp = rclcpp::Time(static_cast<int64_t>(
        pose.m_absoluteTime * 1e6));  // XME time is in milliseconds, convert to nanoseconds

  // Convert XME segment states to common SegmentKinematics map.
  std::unordered_map<std::string, xsens_mvn_ros2::SegmentKinematics> segments;
  for (size_t i = 0; i < pose.m_segmentStates.size(); ++i) {
    const auto & s = pose.m_segmentStates[i];
    const std::string name =
      xmeSegmentToCanonical(xme->segmentName(static_cast<int>(i + 1)).toStdString());

    RCLCPP_DEBUG_STREAM(get_logger(), " * Segment " << name);
    RCLCPP_DEBUG_STREAM(
      get_logger(), std::setprecision(4)
                      << "\tposition:    (" << std::setw(8) << s.m_position[0] << ", "
                      << std::setw(8) << s.m_position[1] << ", " << std::setw(8)
                      << s.m_position[2] << ")");
    RCLCPP_DEBUG_STREAM(
      get_logger(), std::setprecision(4)
                      << "\n\torientation: (" << std::setw(8) << s.m_orientation[0] << ", "
                      << std::setw(8) << s.m_orientation[1] << ", " << std::setw(8)
                      << s.m_orientation[2] << ", " << std::setw(8) << s.m_orientation[3] << ")");

    xsens_mvn_ros2::SegmentKinematics kin;
    kin.position = {s.m_position[0], s.m_position[1], s.m_position[2]};
    kin.orientation = xsens_mvn_ros2::applyArmCorrection(
      name,
      Eigen::Quaterniond(
        s.m_orientation.w(), s.m_orientation.x(), s.m_orientation.y(), s.m_orientation.z()));
    kin.velocity_linear = {s.m_velocity[0], s.m_velocity[1], s.m_velocity[2]};
    kin.velocity_angular = {s.m_angularVelocity[0], s.m_angularVelocity[1], s.m_angularVelocity[2]};
    kin.accel_linear = {s.m_acceleration[0], s.m_acceleration[1], s.m_acceleration[2]};
    kin.accel_angular = {
      s.m_angularAcceleration[0], s.m_angularAcceleration[1], s.m_angularAcceleration[2]};
    segments[name] = kin;
  }
  m_skeletonPub->publishLinkStates(stamp, segments);

  // Convert XME joint angles to common JointAngles vector.
  auto joints = xme->joints();
  auto jointAngles = xme->jointAngles(pose);
  std::vector<xsens_mvn_ros2::JointAngles> joint_vec;
  for (size_t i = 0; i < joints.size(); ++i) {
    if (!joints[i].valid()) {
      continue;
    }
    auto joint_angle = XsVector(jointAngles[i], jointAngles.cols());
    if (joint_angle.size() < 3) {
      continue;
    }
    joint_vec.push_back(
      {xmeSegmentToCanonical(joints[i].name().toStdString()), joint_angle[0], joint_angle[1],
        joint_angle[2]});
  }
  m_skeletonPub->publishJointStates(stamp, joint_vec);

  // Publish center of mass if there are subscribers
  if (m_comPublisher && m_comPublisher->get_subscription_count() > 0) {
    auto comState = xme->centerOfMass(XME_LAST_AVAILABLE_FRAME);
    if (!comState.empty()) {
      Eigen::Vector3d comVec(comState.m_position[0], comState.m_position[1],
        comState.m_position[2]);
      m_comPublisher->publish(tf2::toMsg(comVec));
    }
  }
}

void XsensXmeNode::onHardwareReady(XmeControl * /*control*/)  // NOLINT(readability/casting)
{
  RCLCPP_INFO(get_logger(), "Hardware is ready");
  std::scoped_lock lock(m_hardwareErrorMutex);
  m_lastError.clear();
  m_missingTrackers.clear();
  m_duplicateTrackers.clear();
}

void XsensXmeNode::onHardwareError(XmeControl * xme)
{
  std::scoped_lock lock(m_hardwareErrorMutex);

  auto s = xme->status().suitStatus();

  if (
    s.m_masterDevice.m_deviceId.isAwindaX() && m_awindaChannel != 0 &&
    s.m_wirelessChannel != m_awindaChannel)
  {
    XmeControl::setRadioChannel(
      s.m_masterDevice.m_deviceId, m_awindaChannel);
  }

  XsIntArray dups;
  XsDeviceIdArray dupIds;
  for (auto const & t : s.m_sensors) {
    if (t.m_validity == XDV_Duplicate) {
      dups.push_back(t.m_segmentId);
      dupIds.push_back(t.m_deviceId);
    }
  }

  if (
    m_lastError == s.m_hardwareStatusText && m_missingTrackers == s.m_missingSensors &&
    m_duplicateTrackers == dups)
  {
    RCLCPP_INFO_STREAM_THROTTLE(get_logger(), *get_clock(), 5000, "Hardware not ready");
    return;
  }
  m_lastError = s.m_hardwareStatusText;
  m_missingTrackers = s.m_missingSensors;
  m_duplicateTrackers = dups;

  RCLCPP_WARN_STREAM(get_logger(), "Hardware error: " << s.m_hardwareStatusText);
  if (s.m_hardwareStatus == XHS_MissingSensor) {
    RCLCPP_WARN_STREAM(get_logger(), m_missingTrackers.size() << " missing locations:");
    for (auto i : m_missingTrackers) {
      RCLCPP_WARN_STREAM(get_logger(), " " << i << "(" << xme->segmentName(i) << ")");
    }

    RCLCPP_WARN_STREAM(get_logger(), dups.size() << " duplicate locations:");
    for (XsSize i = 0; i < dups.size(); ++i) {
      RCLCPP_WARN_STREAM(
        get_logger(),
        " [" << dupIds[i].toString() << "-" << dups[i] << "(" << xme->segmentName(dups[i]) << ")]");
    }
  }
}

void XsensXmeNode::onHardwareDisconnected([[maybe_unused]] XmeControl * xme)
{
  logAndPublishFeedback("Hardware was disconnected");
  // Force diagnostics update on disconnect
  if (m_diagnosticUpdater) {
    m_diagnosticUpdater->force_update();
  }
}

void XsensXmeNode::onConfigurationChangeComplete([[maybe_unused]] XmeControl * xme)
{
  RCLCPP_INFO(get_logger(), "Configuration change completed successfully");
  m_configChangeStatus.processStatus = ConfigurationProcessStep::COMPLETED;
  m_configChangeStatus.result = ConfigurationResult::SUCCESS;
}

void XsensXmeNode::onConfigurationChangeFailed([[maybe_unused]] XmeControl * xme)
{
  RCLCPP_ERROR(get_logger(), "Configuration change failed");
  m_configChangeStatus.processStatus = ConfigurationProcessStep::COMPLETED;
  m_configChangeStatus.result = ConfigurationResult::FAILED;
}

}  // namespace xsens_xme_ros2

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<xsens_xme_ros2::XsensXmeNode>();
  rclcpp::executors::SingleThreadedExecutor exec;
  exec.add_node(node->get_node_base_interface());
  exec.spin();
  rclcpp::shutdown();
  return EXIT_SUCCESS;
}
