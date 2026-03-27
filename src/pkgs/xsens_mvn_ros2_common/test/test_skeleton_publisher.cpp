// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
// Tests for SegmentKinematics, JointAngles PODs, arm correction functions,
// and frameName utility. Full SkeletonPublisher publish tests that require
// a live ROS2 node and TF broadcaster live in test_skeleton_publisher_node.cpp.

#include <gtest/gtest.h>
#include "xsens_mvn_ros2_common/skeleton_publisher.hpp"

// ---- SegmentKinematics POD tests ----

TEST(SegmentKinematics, DefaultConstructsToZero)
{
  xsens_mvn_ros2::SegmentKinematics kin;
  EXPECT_EQ(kin.position, Eigen::Vector3d::Zero());
  EXPECT_EQ(kin.velocity_linear, Eigen::Vector3d::Zero());
  EXPECT_EQ(kin.velocity_angular, Eigen::Vector3d::Zero());
  EXPECT_EQ(kin.accel_linear, Eigen::Vector3d::Zero());
  EXPECT_EQ(kin.accel_angular, Eigen::Vector3d::Zero());
}

TEST(SegmentKinematics, DefaultOrientationIsIdentity)
{
  xsens_mvn_ros2::SegmentKinematics kin;
  EXPECT_TRUE(kin.orientation.isApprox(Eigen::Quaterniond::Identity()));
}

// ---- JointAngles POD tests ----

TEST(JointAngles, FieldsStoredCorrectly)
{
  xsens_mvn_ros2::JointAngles ja{"right_elbow", 1.0, 2.0, 3.0};
  EXPECT_EQ(ja.name, "right_elbow");
  EXPECT_DOUBLE_EQ(ja.x, 1.0);
  EXPECT_DOUBLE_EQ(ja.y, 2.0);
  EXPECT_DOUBLE_EQ(ja.z, 3.0);
}

TEST(JointAngles, DefaultsToZero)
{
  xsens_mvn_ros2::JointAngles ja;
  EXPECT_DOUBLE_EQ(ja.x, 0.0);
  EXPECT_DOUBLE_EQ(ja.y, 0.0);
  EXPECT_DOUBLE_EQ(ja.z, 0.0);
  EXPECT_TRUE(ja.name.empty());
}

// ---- Arm correction constant tests ----

TEST(ArmCorrection, RightArmCorrectionIsNeg90X)
{
  const auto & q = xsens_mvn_ros2::rightArmCorrection();
  EXPECT_NEAR(q.w(), 0.7071068, 1e-6);
  EXPECT_NEAR(q.x(), -0.7071068, 1e-6);
  EXPECT_NEAR(q.y(), 0.0, 1e-6);
  EXPECT_NEAR(q.z(), 0.0, 1e-6);
}

TEST(ArmCorrection, LeftArmCorrectionIsPos90X)
{
  const auto & q = xsens_mvn_ros2::leftArmCorrection();
  EXPECT_NEAR(q.w(), 0.7071068, 1e-6);
  EXPECT_NEAR(q.x(), 0.7071068, 1e-6);
  EXPECT_NEAR(q.y(), 0.0, 1e-6);
  EXPECT_NEAR(q.z(), 0.0, 1e-6);
}

TEST(ArmCorrection, CorrectionsAreUnitQuaternions)
{
  EXPECT_NEAR(xsens_mvn_ros2::rightArmCorrection().norm(), 1.0, 1e-6);
  EXPECT_NEAR(xsens_mvn_ros2::leftArmCorrection().norm(), 1.0, 1e-6);
}

// ---- applyArmCorrection tests ----

TEST(ArmCorrection, ApplyToRightArmSegments)
{
  Eigen::Quaterniond q_in = Eigen::Quaterniond::Identity();

  for (const auto & seg : {"right_upper_arm", "right_forearm", "right_hand"}) {
    auto result = xsens_mvn_ros2::applyArmCorrection(seg, q_in);
    EXPECT_TRUE(result.isApprox(xsens_mvn_ros2::rightArmCorrection(), 1e-6))
      << "Failed for segment: " << seg;
  }
}

TEST(ArmCorrection, ApplyToLeftArmSegments)
{
  Eigen::Quaterniond q_in = Eigen::Quaterniond::Identity();

  for (const auto & seg : {"left_upper_arm", "left_forearm", "left_hand"}) {
    auto result = xsens_mvn_ros2::applyArmCorrection(seg, q_in);
    EXPECT_TRUE(result.isApprox(xsens_mvn_ros2::leftArmCorrection(), 1e-6))
      << "Failed for segment: " << seg;
  }
}

TEST(ArmCorrection, NonArmSegmentsUnchanged)
{
  Eigen::Quaterniond q_in(0.5, 0.5, 0.5, 0.5);

  for (const auto & seg :
    {"pelvis", "l5", "t8", "head", "neck", "right_shoulder",
      "left_shoulder", "right_upper_leg", "left_foot"})
  {
    auto result = xsens_mvn_ros2::applyArmCorrection(seg, q_in);
    EXPECT_TRUE(result.isApprox(q_in, 1e-10))
      << "Non-arm segment '" << seg << "' was incorrectly modified";
  }
}

TEST(ArmCorrection, CorrectionIsPostMultiplication)
{
  Eigen::Quaterniond q_in(Eigen::AngleAxisd(M_PI / 4, Eigen::Vector3d::UnitZ()));
  auto result = xsens_mvn_ros2::applyArmCorrection("right_hand", q_in);
  Eigen::Quaterniond expected = q_in * xsens_mvn_ros2::rightArmCorrection();
  EXPECT_TRUE(result.isApprox(expected, 1e-10));
}

// ---- frameName tests ----

TEST(FrameName, ConcatenatesModelAndSegment)
{
  EXPECT_EQ(xsens_mvn_ros2::frameName("skeleton", "pelvis"), "skeleton_pelvis");
  EXPECT_EQ(xsens_mvn_ros2::frameName("robot", "head"), "robot_head");
}

TEST(FrameName, EmptyModelName)
{
  EXPECT_EQ(xsens_mvn_ros2::frameName("", "pelvis"), "_pelvis");
}

TEST(FrameName, EmptySegmentName)
{
  EXPECT_EQ(xsens_mvn_ros2::frameName("skeleton", ""), "skeleton_");
}

int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
