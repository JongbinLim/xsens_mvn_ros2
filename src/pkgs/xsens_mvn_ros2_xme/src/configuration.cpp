// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include <xstypes/xstime.h>

#include "xsens_mvn_ros2_xme/xsens_xme_node.hpp"

constexpr int CONFIGURATION_TIMEOUT_S = 20;

namespace xsens_xme_ros2
{

rcl_interfaces::msg::SetParametersResult XsensXmeNode::onSetParametersCallback(
  const std::vector<rclcpp::Parameter> & parameters)
{
  rcl_interfaces::msg::SetParametersResult result;
  result.successful = true;
  for (const auto & param : parameters) {
    RCLCPP_INFO_STREAM(
      get_logger(),
      "Parameter change request: " << param.get_name() << " -> " << param.value_to_string());
    if (param.get_name() == "sampleRate" && !setSampleRate(param.as_double())) {
      result.successful = false;
      result.reason = "Failed to set sample rate to " + std::to_string(param.as_double()) + " Hz";
      RCLCPP_ERROR_STREAM(get_logger(), "Parameter change rejected: " << result.reason);
      return result;
    } else if (param.get_name() == "suitConfiguration" &&  // NOLINT(readability/braces)
      !setSuitConfiguration(param.as_string()))
    {
      result.successful = false;
      result.reason = "Failed to set suit configuration to " + param.as_string();
      RCLCPP_ERROR_STREAM(get_logger(), "Parameter change rejected: " << result.reason);
      return result;
    } else if (const std::string prefix = "bodyDimension."; param.get_name().find(prefix) == 0) {
      // Extract the body dimension label (e.g., "bodyHeight" from "bodyDimension.bodyHeight")
      std::string label = param.get_name().substr(prefix.length());
      setDimension(label, param.as_double());
    } else if (param.get_name() == "biomechanicalModel" &&  // NOLINT(readability/braces)
      !setBiomechanicalModel(param.as_string()))
    {
      result.successful = false;
      result.reason = "Failed to set biomechanical model to " + param.as_string();
      RCLCPP_ERROR_STREAM(get_logger(), "Parameter change rejected: " << result.reason);
      return result;
    }
  }
  return result;
}

void XsensXmeNode::setDimension(const std::string & label, double value)
{
  m_xmeControl->setBodyDimension(XsString(label), value);
  m_calibrationValid = false;
  RCLCPP_WARN_STREAM(
    get_logger(),
    "Set body dimension " << label << " to " << value <<
      " meters. Calibration invalidated — recalibrate before using pose data.");
}

bool XsensXmeNode::setSampleRate(double rate)
{
  // Reset flags and set to IN_PROGRESS atomically before issuing the SDK call,
  // to prevent the callback from completing before IN_PROGRESS is set.
  m_configChangeStatus.processStatus.store(
    ConfigurationProcessStep::NOT_STARTED, std::memory_order_relaxed);
  m_configChangeStatus.result.store(ConfigurationResult::FAILED, std::memory_order_relaxed);
  m_configChangeStatus.processStatus.store(
    ConfigurationProcessStep::IN_PROGRESS, std::memory_order_release);
  m_xmeControl->setSampleRate(rate);

  // Wait for callback (with timeout)
  auto startTime = std::chrono::steady_clock::now();
  while (m_configChangeStatus.processStatus != ConfigurationProcessStep::COMPLETED &&
    m_xmeControl->lastResultValue() == XRV_OK)
  {
    auto elapsed = std::chrono::steady_clock::now() - startTime;
    if (elapsed > std::chrono::seconds(CONFIGURATION_TIMEOUT_S)) {
      RCLCPP_ERROR(get_logger(), "Timeout waiting for sample rate configuration change callback");
      break;
    }
    RCLCPP_INFO_STREAM_THROTTLE(
      get_logger(), *get_clock(), 1000,
      "Waiting for sample rate configuration change callback... Elapsed time: "
        << std::chrono::duration_cast<std::chrono::seconds>(elapsed).count()
        << " seconds. This procedure may take a while. It is advised to set the sample rate at "
           "launch time.");
  }

  if (m_configChangeStatus.result == ConfigurationResult::SUCCESS) {
    RCLCPP_INFO_STREAM(get_logger(), "Successfully set sample rate to " << rate << " Hz");
  } else {
    auto suit_status = m_xmeControl->status().suitStatus();
    XsDeviceIdArray device_ids;
    device_ids.push_back(suit_status.m_masterDevice.m_deviceId);
    auto allowed_sample_rates = XmeControl::determineSupportedUpdateRates(
      device_ids, static_cast<int>(suit_status.m_sensors.size()), false);
    RCLCPP_ERROR_STREAM(
      get_logger(), "Could not set sample rate to " << rate << " Hz. Allowed sample rates are:");
    for (const auto & allowed_rate : allowed_sample_rates) {
      RCLCPP_ERROR_STREAM(get_logger(), "  " << allowed_rate << " Hz");
    }
    return false;
  }
  return true;
}

bool XsensXmeNode::setSuitConfiguration(const std::string & configName)
{
  // Reset flags and set to IN_PROGRESS atomically before issuing the SDK call.
  m_configChangeStatus.processStatus.store(
    ConfigurationProcessStep::NOT_STARTED, std::memory_order_relaxed);
  m_configChangeStatus.result.store(ConfigurationResult::FAILED, std::memory_order_relaxed);
  m_configChangeStatus.processStatus.store(
    ConfigurationProcessStep::IN_PROGRESS, std::memory_order_release);
  XsString xsSuitConfiguration(configName.c_str());
  m_xmeControl->setConfiguration(xsSuitConfiguration);

  // Wait for callback (with timeout)
  auto startTime = std::chrono::steady_clock::now();
  while (m_configChangeStatus.processStatus != ConfigurationProcessStep::COMPLETED) {
    if (
      std::chrono::steady_clock::now() - startTime >
      std::chrono::seconds(CONFIGURATION_TIMEOUT_S))
    {
      RCLCPP_ERROR(get_logger(), "Timeout waiting for suit configuration change callback");
      return false;
    }
  }

  if (m_configChangeStatus.result == ConfigurationResult::SUCCESS) {
    RCLCPP_INFO_STREAM(get_logger(), "Successfully set suit configuration to " << configName);
  } else {
    auto allowedSuitConfigurations = m_xmeControl->configurationList();
    RCLCPP_ERROR_STREAM(get_logger(), "Could not set suit configuration to " << configName << ".");
    for (const auto & allowedConfig : allowedSuitConfigurations) {
      RCLCPP_ERROR_STREAM(get_logger(), "  " << allowedConfig.c_str());
    }
    return false;
  }

  return true;
}

bool XsensXmeNode::setBiomechanicalModel(const std::string & biomechModel)
{
  if (biomechModel == "Legacy") {
    m_xmeControl->setBiomechanicalModel(XmeBiomechanicalModel::Legacy);
  } else if (biomechModel == "Female") {
    m_xmeControl->setBiomechanicalModel(XmeBiomechanicalModel::Female);
  } else if (biomechModel == "Male") {
    m_xmeControl->setBiomechanicalModel(XmeBiomechanicalModel::Male);
  } else {
    RCLCPP_ERROR_STREAM(
      get_logger(), "Unknown biomechanical model: "
                      << biomechModel << ". Allowed options are: Legacy, Female, Male");
    return false;
  }
  RCLCPP_INFO_STREAM(get_logger(), "Setting biomechanical model to " << biomechModel);
  return true;
}

}  // namespace xsens_xme_ros2
