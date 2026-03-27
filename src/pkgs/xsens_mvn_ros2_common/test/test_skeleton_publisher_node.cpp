// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
// Integration-style unit tests for SkeletonPublisher using a real ROS2 node.
// Verifies TF publishing, relative transform computation, and message output.

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "xsens_mvn_ros2_common/skeleton_publisher.hpp"

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <tf2_ros/buffer.h>                                                // NOLINT(build/include_order)
#include <tf2_ros/transform_broadcaster.h>                                 // NOLINT(build/include_order)
#include <tf2_ros/transform_listener.h>                                    // NOLINT(build/include_order)
#include <xsens_mvn_msgs/msg/link_state_array.hpp>

class SkeletonPublisherTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    node_ = std::make_shared<rclcpp_lifecycle::LifecycleNode>("test_skeleton_pub");
    node_->declare_parameter("model_name", "skeleton");
    node_->declare_parameter("reference_frame", "world");
    tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*node_);
    publisher_ = std::make_shared<xsens_mvn_ros2::SkeletonPublisher>(*node_, *tf_broadcaster_);

    // Activate the node so lifecycle publishers can publish
    node_->configure();
    node_->activate();
    publisher_->activate();

    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(node_->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    link_state_sub_ = node_->create_subscription<xsens_mvn_msgs::msg::LinkStateArray>(
      "link_states", 10,
      [this](xsens_mvn_msgs::msg::LinkStateArray::SharedPtr msg) {
        last_link_states_ = msg;
      });

    joint_state_sub_ = node_->create_subscription<sensor_msgs::msg::JointState>(
      "joint_states", 10,
      [this](sensor_msgs::msg::JointState::SharedPtr msg) {
        last_joint_states_ = msg;
      });
  }

  void spinSome(int ms = 100)
  {
    auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
    while (std::chrono::steady_clock::now() < end) {
      rclcpp::spin_some(node_->get_node_base_interface());
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }

  /// Create a non-identity quaternion (small tilt) to avoid the identity skip logic.
  static Eigen::Quaterniond nonIdentityQuat()
  {
    return Eigen::Quaterniond(Eigen::AngleAxisd(0.01, Eigen::Vector3d::UnitZ()));
  }

  rclcpp_lifecycle::LifecycleNode::SharedPtr node_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  std::shared_ptr<xsens_mvn_ros2::SkeletonPublisher> publisher_;
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  rclcpp::Subscription<xsens_mvn_msgs::msg::LinkStateArray>::SharedPtr link_state_sub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
  xsens_mvn_msgs::msg::LinkStateArray::SharedPtr last_link_states_;
  sensor_msgs::msg::JointState::SharedPtr last_joint_states_;
};

TEST_F(SkeletonPublisherTest, RootSegmentPublishesTFInWorldFrame)
{
  std::unordered_map<std::string, xsens_mvn_ros2::SegmentKinematics> segments;
  auto & pelvis = segments["pelvis"];
  pelvis.position = Eigen::Vector3d(1.0, 2.0, 0.9);
  pelvis.orientation = nonIdentityQuat();

  publisher_->publishLinkStates(node_->now(), segments);
  spinSome(200);

  ASSERT_TRUE(tf_buffer_->canTransform("world", "skeleton_pelvis", tf2::TimePointZero,
    tf2::durationFromSec(0.5)));
  auto tf = tf_buffer_->lookupTransform("world", "skeleton_pelvis", tf2::TimePointZero);
  EXPECT_NEAR(tf.transform.translation.x, 1.0, 1e-3);
  EXPECT_NEAR(tf.transform.translation.y, 2.0, 1e-3);
  EXPECT_NEAR(tf.transform.translation.z, 0.9, 1e-3);
}

TEST_F(SkeletonPublisherTest, IdentityOrientationRootIsSkipped)
{
  std::unordered_map<std::string, xsens_mvn_ros2::SegmentKinematics> segments;
  segments["pelvis"];  // default: identity orientation

  publisher_->publishLinkStates(node_->now(), segments);
  spinSome(200);

  EXPECT_FALSE(tf_buffer_->canTransform("world", "skeleton_pelvis", tf2::TimePointZero,
    tf2::durationFromSec(0.1)));
}

TEST_F(SkeletonPublisherTest, ChildSegmentRelativeToParent)
{
  std::unordered_map<std::string, xsens_mvn_ros2::SegmentKinematics> segments;

  auto & pelvis = segments["pelvis"];
  pelvis.position = Eigen::Vector3d(0.0, 0.0, 1.0);
  pelvis.orientation = nonIdentityQuat();

  // L5 slightly above pelvis, same orientation
  auto & l5 = segments["l5"];
  l5.position = Eigen::Vector3d(0.0, 0.0, 1.1);
  l5.orientation = pelvis.orientation;

  publisher_->publishLinkStates(node_->now(), segments);
  spinSome(200);

  ASSERT_TRUE(tf_buffer_->canTransform("skeleton_pelvis", "skeleton_l5", tf2::TimePointZero,
    tf2::durationFromSec(0.5)));
  auto tf = tf_buffer_->lookupTransform("skeleton_pelvis", "skeleton_l5", tf2::TimePointZero);
  EXPECT_NEAR(tf.transform.translation.z, 0.1, 0.02);
}

TEST_F(SkeletonPublisherTest, MissingParentSegmentIsSkipped)
{
  std::unordered_map<std::string, xsens_mvn_ros2::SegmentKinematics> segments;
  auto & l5 = segments["l5"];
  l5.position = Eigen::Vector3d(0.0, 0.0, 1.0);
  l5.orientation = nonIdentityQuat();

  publisher_->publishLinkStates(node_->now(), segments);
  spinSome(200);

  EXPECT_FALSE(tf_buffer_->canTransform("skeleton_pelvis", "skeleton_l5", tf2::TimePointZero,
    tf2::durationFromSec(0.1)));
}

TEST_F(SkeletonPublisherTest, ChildWithIdentityOrientationIsSkipped)
{
  std::unordered_map<std::string, xsens_mvn_ros2::SegmentKinematics> segments;

  auto & pelvis = segments["pelvis"];
  pelvis.position = Eigen::Vector3d(0.0, 0.0, 1.0);
  pelvis.orientation = nonIdentityQuat();

  // l5 has default identity orientation → should be skipped
  segments["l5"];

  publisher_->publishLinkStates(node_->now(), segments);
  spinSome(200);

  EXPECT_FALSE(tf_buffer_->canTransform("skeleton_pelvis", "skeleton_l5", tf2::TimePointZero,
    tf2::durationFromSec(0.1)));
}

TEST_F(SkeletonPublisherTest, JointStatesPublished)
{
  std::vector<xsens_mvn_ros2::JointAngles> joints = {
    {"l5_s1", 0.1, 0.2, 0.3},
    {"right_elbow", 1.0, 0.0, -1.57},
  };

  publisher_->publishJointStates(node_->now(), joints);
  spinSome(200);

  ASSERT_NE(last_joint_states_, nullptr);
  // Each joint produces 3 entries (x, y, z)
  EXPECT_EQ(last_joint_states_->name.size(), 6u);
  EXPECT_EQ(last_joint_states_->name[0], "skeleton_l5_s1_x");
  EXPECT_EQ(last_joint_states_->name[1], "skeleton_l5_s1_y");
  EXPECT_EQ(last_joint_states_->name[2], "skeleton_l5_s1_z");
  EXPECT_DOUBLE_EQ(last_joint_states_->position[0], 0.1);
  EXPECT_DOUBLE_EQ(last_joint_states_->position[1], 0.2);
  EXPECT_DOUBLE_EQ(last_joint_states_->position[2], 0.3);
}

TEST_F(SkeletonPublisherTest, LinkStateArrayContainsPublishedSegments)
{
  std::unordered_map<std::string, xsens_mvn_ros2::SegmentKinematics> segments;
  auto & pelvis = segments["pelvis"];
  pelvis.position = Eigen::Vector3d(1.0, 2.0, 0.9);
  pelvis.orientation = nonIdentityQuat();
  pelvis.velocity_linear = Eigen::Vector3d(0.1, 0.2, 0.3);
  pelvis.velocity_angular = Eigen::Vector3d(0.4, 0.5, 0.6);

  publisher_->publishLinkStates(node_->now(), segments);
  spinSome(200);

  ASSERT_NE(last_link_states_, nullptr);
  EXPECT_EQ(last_link_states_->states.size(), 1u);
  EXPECT_NEAR(last_link_states_->states[0].pose.position.x, 1.0, 1e-6);
  EXPECT_NEAR(last_link_states_->states[0].twist.linear.x, 0.1, 1e-6);
  EXPECT_NEAR(last_link_states_->states[0].twist.angular.x, 0.4, 1e-6);
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  rclcpp::shutdown();
  return result;
}
