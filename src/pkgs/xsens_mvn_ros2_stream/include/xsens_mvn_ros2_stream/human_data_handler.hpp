// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <Eigen/Dense>

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <rclcpp/logger.hpp>
#include <rclcpp/logging.hpp>

namespace xsens_mvn_ros2
{

class JointState
{
public:
  Eigen::Vector3d angles;
};

class Joint
{
public:
  xsens_mvn_ros2::JointState state;
  std::string name;
  std::string parent_link;
  std::string child_link;

  Joint() = default;
  explicit Joint(std::string joint_name)
  : name(joint_name) {}
};

class Twist
{
public:
  Eigen::Vector3d linear;
  Eigen::Vector3d angular;
};

class LinkState
{
public:
  Eigen::Vector3d position;
  Eigen::Quaterniond orientation;
  xsens_mvn_ros2::Twist velocity;
  xsens_mvn_ros2::Twist acceleration;
};

class Link
{
public:
  Eigen::Vector3d length;
  xsens_mvn_ros2::LinkState state;
  std::string name;
  std::string parent_joint;

  Link() = default;
  explicit Link(std::string link_name)
  : name(link_name) {}
};

class HumanDataHandler
{
public:
  /**
         * @brief Shared pointer to a HumanDataHandler object
         */
  typedef std::shared_ptr<HumanDataHandler> Ptr;
  typedef std::shared_ptr<const HumanDataHandler> ConstPtr;

  /**
         * @brief HumanDataHandler constructor
         * @param logger ROS2 logger used for error/warning messages.
         *               Defaults to a named logger so the class can be used
         *               without a node context.
         */
  explicit HumanDataHandler(
    rclcpp::Logger logger = rclcpp::get_logger("xsens_mvn_ros2.human_data_handler"));

  /**
         * @brief init Initialize the Xsens data handler
         *
         * @return bool Initialization result
         */
  bool init(
    const std::vector<std::string> & link_name_list,
    const std::vector<std::string> & joint_name_list);

  /**
         * @brief Set the state of a desired joint. If it does not exist,
         *        it will be added to the map joint_name-joint_state
         *
         * @param joint_name name of the joint to set.
         * @param joint state of the joint to set.
         *
         * @return bool: Setting result
         */
  bool setJoint(const std::string & joint_name, const xsens_mvn_ros2::Joint & joint);

  /**
         * @brief Set the joint angles vector of a desired joint.
         *        If it does not exist, it will be added to the map
         *        joint_name-joint_state
         *
         * @param joint_name name of the joint to set.
         * @param joint_angles joint rotations x y z.
         *
         * @return bool: Setting result
         */
  bool setJointAngles(const std::string & joint_name, const Eigen::Vector3d & joint_angles);

  /**
         * @brief Get the state of a desired joint. If it does not exist it return false;
         *
         * @param joint_name name of the joint to get.
         *
         * @return bool: Getting result
         */
  bool getJoint(const std::string & joint_name, xsens_mvn_ros2::Joint & joint);

  /**
         * @brief Set the state of a desired link. If it does not
         *        exist, it will be added to the map
         *        link_name-link_state
         *
         * @param link_name name of the link to set.
         * @param link_state state of the link to set.
         *
         * @return bool: Setting result
         */
  bool setLink(const std::string & link_name, const xsens_mvn_ros2::Link & link);

  /**
         * @brief Set the link pose of a desired link. If it does
         *        not exist, it will be added to the map
         *        link_name-link_state
         *
         * @param link_name name of the link to set.
         * @param link_pos link position vector.
         * @param link_orient link quaternions.
         *
         * @return bool: Setting result
         */
  bool setLinkPose(
    const std::string & link_name, const Eigen::Vector3d & link_pos,
    const Eigen::Quaterniond & link_orient);

  /**
         * @brief Set the link state of a desired link. If it does
         *        not exist, it will be added to the map
         *        link_name-link_state
         *
         * @param link_name name of the link to set.
         * @param link_state link state.
         *
         * @return bool: Setting result
         */
  bool setLinkState(const std::string & link_name, const xsens_mvn_ros2::LinkState & link_state);

  /**
         * @brief Get the state of a desired link. If it does not exist it return false;
         *
         * @param link_name name of the link to get.
         *
         * @return bool: Getting result
         */
  bool getLink(const std::string & link_name, xsens_mvn_ros2::Link & link);

  /**
         * @brief Get the link map;
         *
         * @return std::vector<std::string>: Getting link map
         */
  std::map<std::string, xsens_mvn_ros2::Link, std::less<>> getLinks() const;

  /**
         * @brief Get the joint map;
         *
         * @return std::vector<std::string>: Getting joint map
         */
  std::map<std::string, xsens_mvn_ros2::Joint, std::less<>> getJoints() const;

  /**
         * @brief Get the center of mass;
         *
         * @return Eigen::Vector3d: Getting the center of mass
         */
  Eigen::Vector3d getCOM() const;

  /**
         * @brief Set the center of mass;
         *
         * @param Eigen::Vector3d: center of mass position with respect to the reference frame
         */
  void setCOM(const Eigen::Vector3d & com);

private:
  rclcpp::Logger logger_;
  mutable std::mutex data_mutex_;
  std::map<std::string, xsens_mvn_ros2::Joint, std::less<>> joint_map_;
  std::map<std::string, xsens_mvn_ros2::Link, std::less<>> link_map_;

  Eigen::Vector3d com_;

  std::vector<std::string> link_name_list_, joint_name_list_;
};

}  // namespace xsens_mvn_ros2
