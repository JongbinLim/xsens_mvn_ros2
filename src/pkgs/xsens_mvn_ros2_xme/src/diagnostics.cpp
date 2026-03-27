// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include <diagnostic_msgs/msg/diagnostic_status.hpp>

#include "xsens_mvn_ros2_xme/xsens_xme_node.hpp"

namespace xsens_xme_ros2
{
void XsensXmeNode::diagnosticsCallback(diagnostic_updater::DiagnosticStatusWrapper & stat)
{
  if (!m_xmeControl) {
    stat.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "XME control not initialized");
    return;
  }

  if (!m_xmeControl->status().isConnected()) {
    stat.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Hardware not connected");
    return;
  }

  auto suit_status = m_xmeControl->status().suitStatus();
  auto total_sensors = static_cast<int>(suit_status.m_sensors.size());
  const auto masterStatus = suit_status.m_masterDevice;
  stat.add("Total Sensors", total_sensors);
  stat.add("Battery level (%)", masterStatus.m_batteryLevel);
  stat.add("Radio Quality (%)", masterStatus.m_radioQuality);
  stat.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "Hardware connected and operational.");

  // Warn on low battery or poor radio quality.
  // A value of 0 indicates the master device is not reporting this metric.
  if (masterStatus.m_batteryLevel > 0 && masterStatus.m_batteryLevel < 20) {
    stat.mergeSummary(
      diagnostic_msgs::msg::DiagnosticStatus::WARN,
      "Battery level low (" + std::to_string(masterStatus.m_batteryLevel) + "%).");
  }
  if (masterStatus.m_radioQuality > 0 && masterStatus.m_radioQuality < 20) {
    stat.mergeSummary(
      diagnostic_msgs::msg::DiagnosticStatus::WARN,
      "Radio quality low (" + std::to_string(masterStatus.m_radioQuality) + "%).");
  }

  // Calibration validity
  if (!m_calibrationValid) {
    stat.mergeSummary(
      diagnostic_msgs::msg::DiagnosticStatus::ERROR,
      "Calibration invalidated: body dimensions changed. Recalibrate to resume publishing.");
  }

  // Calibration results
  auto npose_calibration_quality = m_xmeControl->calibrationResult(XsString("Npose")).m_quality;
  auto tpose_calibration_quality = m_xmeControl->calibrationResult(XsString("Tpose")).m_quality;
  stat.add("Npose Calibration result", getEnumName(npose_calibration_quality));
  stat.add("Tpose Calibration result", getEnumName(tpose_calibration_quality));
  if (
    (npose_calibration_quality != XCalQ_Good && npose_calibration_quality != XCalQ_Acceptable) ||
    (tpose_calibration_quality != XCalQ_Good && tpose_calibration_quality != XCalQ_Acceptable))
  {
    stat.mergeSummary(
      diagnostic_msgs::msg::DiagnosticStatus::WARN, "Calibration quality is not good enough.");
  }

  if (m_xmeControl->status().isOutOfRange()) {
    stat.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Hardware is out of range");
  }

  // Thread-safe access to hardware error state
  {
    std::scoped_lock lock(m_hardwareErrorMutex);
    if (!m_lastError.empty() || suit_status.m_hardwareStatus != XHS_HardwareOk) {
      stat.add("Last Error: ", m_lastError.c_str());
      stat.add("Hardware Status: ", suit_status.m_hardwareStatusText.c_str());
      if (!m_missingTrackers.empty()) {
        stat.add("Missing Sensors", static_cast<int>(m_missingTrackers.size()));
      }
      if (!m_duplicateTrackers.empty()) {
        stat.add("Duplicate Sensors", static_cast<int>(m_duplicateTrackers.size()));
      }
      stat.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Hardware error detected");
    }
  }
}
}  // namespace xsens_xme_ros2
