// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include "xsens_mvn_ros2_stream/human_data_handler.hpp"

#include <string>
#include <vector>

namespace xsens_mvn_ros2
{

HumanDataHandler::HumanDataHandler(rclcpp::Logger logger)
: logger_(logger)
{
  com_ << 0.0, 0.0, 0.0;
}

bool HumanDataHandler::init(
  const std::vector<std::string> & link_name_list, const std::vector<std::string> & joint_name_list)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  link_name_list_ = link_name_list;
  joint_name_list_ = joint_name_list;

  link_map_.clear();
  for (std::string link_name : link_name_list_) {
    link_map_.try_emplace(link_name, xsens_mvn_ros2::Link(link_name));
  }

  joint_map_.clear();
  for (std::string joint_name : joint_name_list_) {
    joint_map_.try_emplace(joint_name, xsens_mvn_ros2::Joint(joint_name));
  }

  return true;
}

bool HumanDataHandler::setJoint(
  const std::string & joint_name, const xsens_mvn_ros2::Joint & joint)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  if (joint_map_.count(joint_name) == 0) {
    joint_map_.try_emplace(joint_name, joint);
  } else {
    joint_map_[joint_name] = joint;
  }

  return true;
}

bool HumanDataHandler::setJointAngles(
  const std::string & joint_name, const Eigen::Vector3d & joint_angles)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  if (joint_map_.count(joint_name) == 0) {
    xsens_mvn_ros2::Joint joint;
    joint.name = joint_name;
    joint.state.angles = joint_angles;
    joint_map_.try_emplace(joint_name, joint);
  } else {
    joint_map_[joint_name].state.angles = joint_angles;
  }

  return true;
}

bool HumanDataHandler::getJoint(const std::string & joint_name, xsens_mvn_ros2::Joint & joint)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  if (joint_map_.count(joint_name) == 0) {
    RCLCPP_WARN(logger_, "Joint \"%s\" not found.", joint_name.c_str());
    return false;
  }

  joint = joint_map_[joint_name];

  return true;
}

bool HumanDataHandler::setLink(const std::string & link_name, const xsens_mvn_ros2::Link & link)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  auto normalized_link = link;
  normalized_link.state.orientation = normalized_link.state.orientation.normalized();

  if (link_map_.count(link_name) == 0) {
    link_map_.try_emplace(link_name, normalized_link);
  } else {
    link_map_[link_name] = normalized_link;
  }

  return true;
}

bool HumanDataHandler::setLinkPose(
  const std::string & link_name, const Eigen::Vector3d & link_pos,
  const Eigen::Quaterniond & link_orient)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  if (link_map_.count(link_name) == 0) {
    xsens_mvn_ros2::Link link;
    link.name = link_name;
    link.state.position = link_pos;
    link.state.orientation = link_orient.normalized();
    link_map_.try_emplace(link_name, link);
  } else {
    link_map_[link_name].state.position = link_pos;
    link_map_[link_name].state.orientation = link_orient.normalized();
  }

  return true;
}

bool HumanDataHandler::setLinkState(
  const std::string & link_name, const xsens_mvn_ros2::LinkState & link_state)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  auto normalized_state = link_state;
  normalized_state.orientation = normalized_state.orientation.normalized();

  if (link_map_.count(link_name) == 0) {
    xsens_mvn_ros2::Link link;
    link.name = link_name;
    link.state = normalized_state;
    link_map_.try_emplace(link_name, link);
  } else {
    link_map_[link_name].state = normalized_state;
  }

  return true;
}

bool HumanDataHandler::getLink(const std::string & link_name, xsens_mvn_ros2::Link & link)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  if (link_map_.count(link_name) == 0) {
    RCLCPP_WARN(logger_, "Link \"%s\" not found.", link_name.c_str());
    return false;
  }

  link = link_map_[link_name];

  return true;
}

std::map<std::string, xsens_mvn_ros2::Link, std::less<>> HumanDataHandler::getLinks() const
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  return link_map_;
}

std::map<std::string, xsens_mvn_ros2::Joint, std::less<>> HumanDataHandler::getJoints() const
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  return joint_map_;
}

Eigen::Vector3d HumanDataHandler::getCOM() const
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  return com_;
}

void HumanDataHandler::setCOM(const Eigen::Vector3d & com)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  com_ = com;
}

}  // namespace xsens_mvn_ros2
