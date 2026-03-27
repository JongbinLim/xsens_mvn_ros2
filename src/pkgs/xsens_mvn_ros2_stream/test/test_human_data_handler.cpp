// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include <gtest/gtest.h>

#include <Eigen/Geometry>
#include <thread>
#include <vector>

#include "xsens_mvn_ros2_stream/human_data_handler.hpp"

namespace
{
rclcpp::Logger test_logger()
{
  static auto logger = rclcpp::get_logger("test_human_data_handler");
  return logger;
}
}  // namespace

// ---- Basic set/get roundtrip tests ----

TEST(HumanDataHandler, SetGetLinkRoundtrip)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  xsens_mvn_ros2::Link link("pelvis");
  link.state.position = Eigen::Vector3d(1.0, 2.0, 3.0);
  link.state.orientation = Eigen::Quaterniond(1.0, 0.0, 0.0, 0.0);

  ASSERT_TRUE(handler.setLink("pelvis", link));

  xsens_mvn_ros2::Link retrieved;
  ASSERT_TRUE(handler.getLink("pelvis", retrieved));
  EXPECT_EQ(retrieved.name, "pelvis");
  EXPECT_DOUBLE_EQ(retrieved.state.position.x(), 1.0);
  EXPECT_DOUBLE_EQ(retrieved.state.position.y(), 2.0);
  EXPECT_DOUBLE_EQ(retrieved.state.position.z(), 3.0);
}

TEST(HumanDataHandler, SetGetJointRoundtrip)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  xsens_mvn_ros2::Joint joint("l5_s1");

  ASSERT_TRUE(handler.setJoint("l5_s1", joint));

  xsens_mvn_ros2::Joint retrieved;
  ASSERT_TRUE(handler.getJoint("l5_s1", retrieved));
  EXPECT_EQ(retrieved.name, "l5_s1");
}

TEST(HumanDataHandler, GetMissingLinkReturnsFalse)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  xsens_mvn_ros2::Link retrieved;
  EXPECT_FALSE(handler.getLink("nonexistent", retrieved));
}

TEST(HumanDataHandler, GetMissingJointReturnsFalse)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  xsens_mvn_ros2::Joint retrieved;
  EXPECT_FALSE(handler.getJoint("nonexistent", retrieved));
}

TEST(HumanDataHandler, SetGetCOM)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  Eigen::Vector3d com(0.5, 0.0, 0.9);
  handler.setCOM(com);

  auto result = handler.getCOM();
  EXPECT_DOUBLE_EQ(result.x(), 0.5);
  EXPECT_DOUBLE_EQ(result.y(), 0.0);
  EXPECT_DOUBLE_EQ(result.z(), 0.9);
}

TEST(HumanDataHandler, COMDefaultsToZero)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  auto com = handler.getCOM();
  EXPECT_DOUBLE_EQ(com.x(), 0.0);
  EXPECT_DOUBLE_EQ(com.y(), 0.0);
  EXPECT_DOUBLE_EQ(com.z(), 0.0);
}

TEST(HumanDataHandler, GetLinksReturnsInsertedLinks)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  handler.setLink("pelvis", xsens_mvn_ros2::Link("pelvis"));
  handler.setLink("l5", xsens_mvn_ros2::Link("l5"));

  auto links = handler.getLinks();
  EXPECT_EQ(links.size(), 2u);
  EXPECT_NE(links.find("pelvis"), links.end());
  EXPECT_NE(links.find("l5"), links.end());
}

TEST(HumanDataHandler, GetJointsReturnsAllJoints)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  handler.setJoint("l5_s1", xsens_mvn_ros2::Joint("l5_s1"));
  handler.setJoint("t9_t8", xsens_mvn_ros2::Joint("t9_t8"));

  auto joints = handler.getJoints();
  EXPECT_EQ(joints.size(), 2u);
  EXPECT_NE(joints.find("l5_s1"), joints.end());
  EXPECT_NE(joints.find("t9_t8"), joints.end());
}

// ---- init() tests ----

TEST(HumanDataHandler, InitPopulatesLinksAndJoints)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  std::vector<std::string> links = {"pelvis", "l5", "t8"};
  std::vector<std::string> joints = {"l5_s1", "t9_t8"};

  ASSERT_TRUE(handler.init(links, joints));

  auto link_map = handler.getLinks();
  EXPECT_EQ(link_map.size(), 3u);
  EXPECT_NE(link_map.find("pelvis"), link_map.end());
  EXPECT_NE(link_map.find("l5"), link_map.end());
  EXPECT_NE(link_map.find("t8"), link_map.end());

  auto joint_map = handler.getJoints();
  EXPECT_EQ(joint_map.size(), 2u);
  EXPECT_NE(joint_map.find("l5_s1"), joint_map.end());
  EXPECT_NE(joint_map.find("t9_t8"), joint_map.end());
}

TEST(HumanDataHandler, InitClearsPreviousData)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  handler.setLink("old_link", xsens_mvn_ros2::Link("old_link"));
  handler.setJoint("old_joint", xsens_mvn_ros2::Joint("old_joint"));

  handler.init({"new_link"}, {"new_joint"});

  auto links = handler.getLinks();
  EXPECT_EQ(links.size(), 1u);
  EXPECT_EQ(links.find("old_link"), links.end());
  EXPECT_NE(links.find("new_link"), links.end());

  auto joints = handler.getJoints();
  EXPECT_EQ(joints.size(), 1u);
  EXPECT_EQ(joints.find("old_joint"), joints.end());
}

// ---- setJointAngles tests ----

TEST(HumanDataHandler, SetJointAnglesCreatesNewJoint)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  Eigen::Vector3d angles(1.0, 2.0, 3.0);

  ASSERT_TRUE(handler.setJointAngles("new_joint", angles));

  xsens_mvn_ros2::Joint retrieved;
  ASSERT_TRUE(handler.getJoint("new_joint", retrieved));
  EXPECT_EQ(retrieved.name, "new_joint");
  EXPECT_DOUBLE_EQ(retrieved.state.angles.x(), 1.0);
  EXPECT_DOUBLE_EQ(retrieved.state.angles.y(), 2.0);
  EXPECT_DOUBLE_EQ(retrieved.state.angles.z(), 3.0);
}

TEST(HumanDataHandler, SetJointAnglesUpdatesExisting)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  handler.setJoint("l5_s1", xsens_mvn_ros2::Joint("l5_s1"));

  Eigen::Vector3d angles(10.0, 20.0, 30.0);
  ASSERT_TRUE(handler.setJointAngles("l5_s1", angles));

  xsens_mvn_ros2::Joint retrieved;
  ASSERT_TRUE(handler.getJoint("l5_s1", retrieved));
  EXPECT_DOUBLE_EQ(retrieved.state.angles.x(), 10.0);
  EXPECT_DOUBLE_EQ(retrieved.state.angles.y(), 20.0);
  EXPECT_DOUBLE_EQ(retrieved.state.angles.z(), 30.0);
}

// ---- setLinkPose tests ----

TEST(HumanDataHandler, SetLinkPoseNormalizesQuaternion)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  Eigen::Vector3d pos(1.0, 2.0, 3.0);
  Eigen::Quaterniond non_unit(2.0, 0.0, 0.0, 0.0);

  ASSERT_TRUE(handler.setLinkPose("pelvis", pos, non_unit));

  xsens_mvn_ros2::Link retrieved;
  ASSERT_TRUE(handler.getLink("pelvis", retrieved));
  EXPECT_NEAR(retrieved.state.orientation.norm(), 1.0, 1e-10);
  EXPECT_NEAR(retrieved.state.orientation.w(), 1.0, 1e-10);
}

TEST(HumanDataHandler, SetLinkPoseUpdatesExisting)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  handler.setLink("pelvis", xsens_mvn_ros2::Link("pelvis"));

  Eigen::Vector3d pos(5.0, 6.0, 7.0);
  Eigen::Quaterniond orient(1.0, 0.0, 0.0, 0.0);
  ASSERT_TRUE(handler.setLinkPose("pelvis", pos, orient));

  xsens_mvn_ros2::Link retrieved;
  ASSERT_TRUE(handler.getLink("pelvis", retrieved));
  EXPECT_DOUBLE_EQ(retrieved.state.position.x(), 5.0);
  EXPECT_DOUBLE_EQ(retrieved.state.position.y(), 6.0);
  EXPECT_DOUBLE_EQ(retrieved.state.position.z(), 7.0);
}

TEST(HumanDataHandler, SetLinkPoseCreatesNewLink)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  Eigen::Vector3d pos(1.0, 2.0, 3.0);
  Eigen::Quaterniond orient = Eigen::Quaterniond::Identity();

  ASSERT_TRUE(handler.setLinkPose("new_link", pos, orient));

  xsens_mvn_ros2::Link retrieved;
  ASSERT_TRUE(handler.getLink("new_link", retrieved));
  EXPECT_EQ(retrieved.name, "new_link");
  EXPECT_DOUBLE_EQ(retrieved.state.position.x(), 1.0);
}

// ---- setLinkState tests ----

TEST(HumanDataHandler, SetLinkStateNormalizesQuaternionOnUpdate)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  handler.setLink("pelvis", xsens_mvn_ros2::Link("pelvis"));

  xsens_mvn_ros2::LinkState state;
  state.position = Eigen::Vector3d(1.0, 2.0, 3.0);
  state.orientation = Eigen::Quaterniond(2.0, 0.0, 0.0, 0.0);

  ASSERT_TRUE(handler.setLinkState("pelvis", state));

  xsens_mvn_ros2::Link retrieved;
  ASSERT_TRUE(handler.getLink("pelvis", retrieved));
  EXPECT_NEAR(retrieved.state.orientation.norm(), 1.0, 1e-10);
}

TEST(HumanDataHandler, SetLinkStateCreatesNewLink)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());

  xsens_mvn_ros2::LinkState state;
  state.position = Eigen::Vector3d(1.0, 2.0, 3.0);
  state.orientation = Eigen::Quaterniond::Identity();

  ASSERT_TRUE(handler.setLinkState("new_link", state));

  xsens_mvn_ros2::Link retrieved;
  ASSERT_TRUE(handler.getLink("new_link", retrieved));
  EXPECT_EQ(retrieved.name, "new_link");
  EXPECT_DOUBLE_EQ(retrieved.state.position.x(), 1.0);
}

// ---- Update semantics tests ----

TEST(HumanDataHandler, SetLinkUpdatesExisting)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  xsens_mvn_ros2::Link link1("pelvis");
  link1.state.position = Eigen::Vector3d(1.0, 0.0, 0.0);
  handler.setLink("pelvis", link1);

  xsens_mvn_ros2::Link link2("pelvis");
  link2.state.position = Eigen::Vector3d(9.0, 8.0, 7.0);
  handler.setLink("pelvis", link2);

  xsens_mvn_ros2::Link retrieved;
  ASSERT_TRUE(handler.getLink("pelvis", retrieved));
  EXPECT_DOUBLE_EQ(retrieved.state.position.x(), 9.0);
  EXPECT_DOUBLE_EQ(retrieved.state.position.y(), 8.0);
  EXPECT_DOUBLE_EQ(retrieved.state.position.z(), 7.0);
}

TEST(HumanDataHandler, SetJointUpdatesExisting)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  xsens_mvn_ros2::Joint joint1("l5_s1");
  joint1.parent_link = "pelvis";
  handler.setJoint("l5_s1", joint1);

  xsens_mvn_ros2::Joint joint2("l5_s1");
  joint2.parent_link = "l5";
  handler.setJoint("l5_s1", joint2);

  xsens_mvn_ros2::Joint retrieved;
  ASSERT_TRUE(handler.getJoint("l5_s1", retrieved));
  EXPECT_EQ(retrieved.parent_link, "l5");
}

// ---- Thread safety test ----

TEST(HumanDataHandler, ConcurrentSetGetDoesNotCrash)
{
  xsens_mvn_ros2::HumanDataHandler handler(test_logger());
  handler.init({"pelvis", "l5"}, {"l5_s1"});

  constexpr int kIterations = 1000;
  auto writer = [&]() {
      for (int i = 0; i < kIterations; i++) {
        Eigen::Vector3d pos(i, i + 1, i + 2);
        Eigen::Quaterniond orient = Eigen::Quaterniond::Identity();
        handler.setLinkPose("pelvis", pos, orient);
        handler.setCOM(pos);
        handler.setJointAngles("l5_s1", pos);
      }
    };

  auto reader = [&]() {
      for (int i = 0; i < kIterations; i++) {
        xsens_mvn_ros2::Link link;
        handler.getLink("pelvis", link);
        handler.getCOM();
        handler.getLinks();
        handler.getJoints();
      }
    };

  std::thread t1(writer);
  std::thread t2(reader);
  std::thread t3(writer);

  t1.join();
  t2.join();
  t3.join();
  SUCCEED();
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  rclcpp::shutdown();
  return result;
}
