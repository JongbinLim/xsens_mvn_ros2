// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <xsens_mvn_sdk/parsermanager.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/thread.hpp>
#include <rclcpp/logger.hpp>
#include <rclcpp/logging.hpp>
#include <xsens_mvn_ros2_common/i_motion_capture_source.hpp>
#include <xsens_mvn_ros2_stream/human_data_handler.hpp>
#include <xsens_mvn_ros2_stream/socket.hpp>

constexpr int MAX_MVN_DATAGRAM_SIZE = 5000;

class XsensStreamClient : public xsens_mvn_ros2::IMotionCaptureSource
{
public:
  XsensStreamClient(rclcpp::Logger logger, const int & udp_port);
  ~XsensStreamClient() override;
  bool init();

  // IMotionCaptureSource interface
  std::unordered_map<std::string, xsens_mvn_ros2::SegmentKinematics> getSegments() const override;
  std::vector<xsens_mvn_ros2::JointAngles> getJoints() const override;
  std::optional<Eigen::Vector3d> getCOM() const override;
  int64_t lastDataTimeNs() const override
  {
    return m_lastDataTimeNs;
  }
  bool isActive() const override
  {
    return m_clientActive;
  }

private:
  rclcpp::Logger m_logger;
  int m_udpPort;
  std::shared_ptr<Socket> m_udpSocket;
  ParserManager m_parserManager;
  xsens_mvn_ros2::HumanDataHandler::Ptr m_humanData;
  std::vector<std::string> m_linkNameList, m_jointNameList;

  boost::thread m_dataAcquisitionThread;
  void dataAcquisitionCallback();
  char m_dataBuffer[MAX_MVN_DATAGRAM_SIZE];
  std::atomic<bool> m_clientActive{false};
  mutable std::mutex m_dataMutex;
  std::atomic<int64_t> m_lastDataTimeNs{0};

  bool buildXsensModel();
  QuaternionDatagram waitForQuaternionDatagram();
  JointAnglesDatagram waitForJointAnglesDatagram();

  bool readData();
  void updateJointAngles();
  void updateLinkPoses();
  void updateLinkLinearTwists();
  void updateLinkAngularTwists();
  void updateCOM();

  Eigen::Vector3d jointAngleToEigenVector3d(
    const JointAngle & joint_angle, const double & x_axis, const double & y_axis,
    const double & z_axis) const;
  void rotateLink(const std::string & link_name, const Eigen::Quaterniond & quat) const;
};
