// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include <xstypes/xstime.h>

#include <chrono>
#include <filesystem>

#include "xsens_mvn_ros2_xme/xsens_xme_node.hpp"

namespace xsens_xme_ros2
{

void XsensXmeNode::setCalibrationpath(
  const xsens_mvn_msgs::srv::LoadCalibrationPath::Request::SharedPtr request,
  xsens_mvn_msgs::srv::LoadCalibrationPath::Response::SharedPtr response)
{
  RCLCPP_INFO_STREAM(
    get_logger(),
    "Received request to set calibration file path to: " << request->calibration_file_path);
  XsString new_calibration_path(request->calibration_file_path.c_str());
  m_xmeControl->loadCalibrations(new_calibration_path);
  switch (m_xmeControl->lastResultValue()) {
    case XRV_OK:
      // Success
      response->success = true;
      response->message =
        "Successfully loaded calibration file: " +
        request->calibration_file_path;
      m_calibrationValid = true;
      break;
    case XRV_NOTFOUND:
      response->message =
        "Calibration file not found at: " +
        request->calibration_file_path;
      response->success = false;
      break;
    case XRV_FILEERROR:
      response->message =
        "Error reading calibration file at: " +
        request->calibration_file_path;
      response->success = false;
      break;
    default:
      response->message =
        getEnumName(m_xmeControl->lastResultValue()) +
        " error loading calibration file at: " +
        request->calibration_file_path;
      response->success = false;
      break;
  }
  if (!response->success) {
    RCLCPP_ERROR_STREAM(get_logger(), response->message);
  } else {
    RCLCPP_INFO_STREAM(get_logger(), response->message);
  }
}

void XsensXmeNode::onCalibrationRecordingSectionChanged(
  [[maybe_unused]] XmeControl * xme, XmeCalibrationRecordingSection calibSection)
{
  logAndPublishFeedback("Calibration recording section changed to " + getEnumName(calibSection));
  m_calibSection = calibSection;
}

void XsensXmeNode::onProgressUpdate(
  [[maybe_unused]] XmeControl * xme, int percentage, XsString const * category)
{
  logAndPublishFeedback(
    (std::stringstream() << category->toStdString() << ": " << percentage << "%").str());
}

void XsensXmeNode::onProcessingProgress(
  [[maybe_unused]] XmeControl * xme, XmeProcessingStage stage, int firstFrame, int lastFrame)
{
  logAndPublishFeedback((std::stringstream() << "Processing stage " << getEnumName(stage) << ": "
                                             << firstFrame << "-" << lastFrame)
    .str());
}

void XsensXmeNode::onCalibrationProcessed([[maybe_unused]] XmeControl * xme)
{
  m_calibrationProcessed = true;
  logAndPublishFeedback("Calibration processed");
}

void XsensXmeNode::onCalibrationAborted([[maybe_unused]] XmeControl * xme)
{
  logAndPublishFeedback("Calibration aborted");
  m_calibrationProcessed = false;
  m_calibrationComplete = false;
}

void XsensXmeNode::onCalibrationComplete([[maybe_unused]] XmeControl * xme)
{
  logAndPublishFeedback("Calibration complete");
  m_calibrationComplete = true;
}

rclcpp_action::GoalResponse XsensXmeNode::handleGoal(
  [[maybe_unused]] const rclcpp_action::GoalUUID & uuid,
  [[maybe_unused]] std::shared_ptr<const PerformCalibration::Goal> goal)
{
  RCLCPP_INFO(get_logger(), "Received goal request to perform calibration");
  if (get_current_state().id() != lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE) {
    RCLCPP_WARN(get_logger(), "Node is not active, rejecting calibration request.");
    return rclcpp_action::GoalResponse::REJECT;
  }
  if (m_activeGoalHandle) {
    RCLCPP_WARN(get_logger(), "A calibration is already in progress, rejecting new requests.");
    return rclcpp_action::GoalResponse::REJECT;
  }
  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse XsensXmeNode::handleCancel(
  [[maybe_unused]] const std::shared_ptr<GoalHandlePerformCalibration> goalHandle) const
{
  RCLCPP_INFO(get_logger(), "Received request to cancel calibration");
  // Accept all cancel requests
  return rclcpp_action::CancelResponse::ACCEPT;
}

void XsensXmeNode::handleAccepted(const std::shared_ptr<GoalHandlePerformCalibration> goalHandle)
{
  // Join any previously completed calibration thread before starting a new one.
  if (m_calibrationThread.joinable()) {
    m_calibrationThread.join();
  }
  m_calibrationThread = std::thread([this, goalHandle]() {execute(goalHandle);});
}

void XsensXmeNode::execute(const std::shared_ptr<GoalHandlePerformCalibration> goalHandle)
{
  // Store the active goal handle so callbacks can access it
  m_activeGoalHandle = goalHandle;
  auto result = std::make_shared<PerformCalibration::Result>();
  RCLCPP_INFO(get_logger(), "Starting calibration execution...");
  XsString pose_type;
  switch (goalHandle->get_goal()->pose) {
    case PerformCalibration::Goal::NPOSE:
      logAndPublishFeedback("Performing N-pose calibration");
      pose_type = "Npose";
      break;
    case PerformCalibration::Goal::TPOSE:
      logAndPublishFeedback("Performing T-pose calibration");
      pose_type = "Tpose";
      break;
    default:
      logAndPublishFeedback("Unknown pose type requested, defaulting to N-pose");
      pose_type = "Npose";
      break;
  }
  m_xmeControl->initializeCalibration(pose_type);
  auto times = m_xmeControl->calibrationRecordingTimesRemaining();
  if (times.empty()) {
    abortActionGoal("Unknown calibration format");
    return;
  } else {
    auto statusMsg = std::stringstream()
                     << "Prepare to start calibration in "
                     << get_parameter("userCalibrationPreparationTimeMs").as_int()
                     << " miliseconds. You will have to stand still in " << pose_type.c_str()
                     << " for " << times[0] / 1000 << " seconds";
    if (times.size() > 1) {
      statusMsg << "and move around for "
                << times[1] / 1000 << " seconds.";
    }
    for (size_t i = 2; i < times.size(); ++i) {
      statusMsg << " There is an unknown section requiring "
                << times[i] / 1000 << " seconds.";
    }
    logAndPublishFeedback(statusMsg.str());
    XsTime::msleep(get_parameter("userCalibrationPreparationTimeMs").as_int());
  }

  m_errorReceived = false;
  m_calibrationProcessed = false;
  m_calibrationComplete = false;

  m_xmeControl->startCalibration(XsTimeStamp::now());
  if (!waitForRecordingState()) {
    abortActionGoal("Calibration failed to start, aborting.");
    return;
  }

  XsSize originalStages = times.size();
  while (!(times = m_xmeControl->calibrationRecordingTimesRemaining()).empty()) {
    if (checkIfGoalCanceled()) {
      return;
    }
    std::stringstream statusMsg;
    if (m_calibSection == XmeCalibrationRecordingSection::Static) {
      if (m_calibrationProcessed) {
        // done with static part, recording dynamic part
        statusMsg << "Static calibration complete, starting dynamic phase";
      } else if (originalStages != times.size()) {
        // done recording but waiting for processing to complete,
        // remain standing still until the results have been applied
        statusMsg << "Please stand still in " << pose_type.c_str();
      } else {
        // recording
        statusMsg << "Please stand still in " << pose_type.c_str() << " for "
                  << std::to_string(times[0] / 1000.0) << " more seconds";
      }
    } else if (m_calibSection == XmeCalibrationRecordingSection::Dynamic) {
      // recording dynamic part
      statusMsg << "Please move around for " << std::to_string(times[0] / 1000.0)
                << " more seconds";
    } else {
      // Recording other section of calibration
      statusMsg << "Recording section " << getEnumName(m_calibSection) << " for "
                << std::to_string(times[0] / 1000.0) << " more seconds";
    }
    logAndPublishFeedback(statusMsg.str());
    XsTime::msleep(1000);
  }

  // Send feedback for processing phase
  logAndPublishFeedback("Processing calibration data...");

  while (!m_calibrationComplete ||
    m_xmeControl->status().isCalibrating())       // both conditions are ok to check
  {
    if (checkIfGoalCanceled()) {
      return;
    }
    XsTime::msleep(10);
  }
  // Send final feedback
  logAndPublishFeedback("Calibration processing complete");

  auto resultMsg = getCalibrationResults(m_xmeControl->calibrationResult(pose_type));

  // Save calibration file
  const auto & output_file = goalHandle->get_goal()->output_file_name;
  try {
    m_xmeControl->createMvnFile(output_file);
    if (m_xmeControl->lastResultValue() != XRV_OK) {
      auto err = "Failed to save calibration file '" + output_file + "': " +
        getEnumName(m_xmeControl->lastResultValue()) +
        ". Check disk space and write permissions.";
      RCLCPP_ERROR_STREAM(get_logger(), err);
      resultMsg << "\nWARNING: " << err;
    } else {
      auto cwd = std::filesystem::current_path().string();
      resultMsg << "\nCalibration file saved to: " << cwd << "/" << output_file;
    }
  } catch (const std::exception & e) {
    auto err = std::string("Exception saving calibration file: ") + e.what() +
      ". Check disk space and write permissions.";
    RCLCPP_ERROR_STREAM(get_logger(), err);
    resultMsg << "\nWARNING: " << err;
  }

  result->calibration_result = resultMsg.str();
  RCLCPP_INFO_STREAM(get_logger(), resultMsg.str());
  m_calibrationValid = true;
  goalHandle->succeed(result);

  // Clear the active goal handle
  m_activeGoalHandle.reset();
}

bool XsensXmeNode::checkIfGoalCanceled()
{
  // Take a local copy to prevent use-after-free if another thread resets m_activeGoalHandle
  auto handle = m_activeGoalHandle;
  if (handle && handle->is_canceling()) {
    m_xmeControl->abortCalibration();
    auto result = std::make_shared<PerformCalibration::Result>();
    result->calibration_result = "Calibration canceled by client.";
    RCLCPP_INFO_STREAM(get_logger(), result->calibration_result);
    handle->canceled(result);
    m_activeGoalHandle.reset();
    return true;
  }
  return false;
}

void XsensXmeNode::abortActionGoal(const std::string & calibrationResult)
{
  if (!m_activeGoalHandle) {
    RCLCPP_WARN(get_logger(), "No active goal handle to abort.");
    return;
  }
  m_xmeControl->abortCalibration();
  auto result = std::make_shared<PerformCalibration::Result>();
  result->calibration_result = std::string("Calibration aborted: ") + calibrationResult;
  RCLCPP_WARN_STREAM(get_logger(), result->calibration_result);
  m_activeGoalHandle->abort(result);
  m_activeGoalHandle.reset();
}

std::stringstream XsensXmeNode::getCalibrationResults(
  const XmeCalibrationResult & calibrationResult) const
{
  std::stringstream resultMsg;
  resultMsg << "Calibration Results:\n";
  switch (calibrationResult.m_quality) {
    case XCalQ_Unknown:
      resultMsg << "unknown";
      break;
    case XCalQ_Good:
      resultMsg << "good";
      break;
    case XCalQ_Acceptable:
      resultMsg << "acceptable";
      break;
    case XCalQ_Poor:
      resultMsg << "poor";
      break;
    case XCalQ_Failed:
      resultMsg << "failed";
      break;
    default:
      resultMsg << "<error>";
      break;
  }
  if (const auto & warnings = calibrationResult.m_warnings; !warnings.empty()) {
    resultMsg << "\nCalibration produced warnings:\n";
    for (auto const & it : warnings) {
      resultMsg << "\t" << it.c_str() << "\n";
    }
  }
  return resultMsg;
}

bool XsensXmeNode::waitForRecordingState() const
{
  XsTime::msleep(100);  // wait a bit for the command to be handled
  auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(30);
  while (!m_errorReceived) {
    if (std::chrono::steady_clock::now() > deadline) {
      RCLCPP_ERROR(
        get_logger(),
        "Timeout waiting for calibration recording state after 30 seconds. "
        "Check: (1) Is the hardware connected and ready? "
        "(2) Are all required sensors present?");
      return false;
    }
    auto state = m_xmeControl->status().recordingState();
    switch (state) {
      default:  // unknown future cases?
      case XRS_NotRecording:
        RCLCPP_WARN(
          get_logger(), "Calibration failed to start (recording state: NotRecording). "
          "Hardware may have reported an error.");
        return false;  // we should at least be waiting to start
      case XRS_WaitingForStart:
        XsTime::msleep(10);
        continue;
      case XRS_Recording:
      case XRS_Flushing:
        return true;
    }
  }
  RCLCPP_WARN(get_logger(), "Hardware error received during calibration startup.");
  return false;
}

void XsensXmeNode::logAndPublishFeedback(const std::string & statusMsg) const
{
  RCLCPP_INFO_STREAM(get_logger(), statusMsg);
  if (m_activeGoalHandle) {
    auto feedback = std::make_shared<PerformCalibration::Feedback>();
    feedback->status = statusMsg;
    m_activeGoalHandle->publish_feedback(feedback);
  }
}

}  // namespace xsens_xme_ros2
