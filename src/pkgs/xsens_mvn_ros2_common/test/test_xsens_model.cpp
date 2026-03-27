// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include <gtest/gtest.h>

#include <unordered_set>

#include "xsens_mvn_ros2_common/xsens_model.hpp"

// ---- Canonical mapping tests ----

TEST(XsensModelTest, CanonicalMappingKnownSegments)
{
  EXPECT_EQ(xmeSegmentToCanonical("Pelvis"), "pelvis");
  EXPECT_EQ(xmeSegmentToCanonical("L5"), "l5");
  EXPECT_EQ(xmeSegmentToCanonical("T8"), "t8");
  EXPECT_EQ(xmeSegmentToCanonical("Sternum"), "t8");
  EXPECT_EQ(xmeSegmentToCanonical("Head"), "head");
  EXPECT_EQ(xmeSegmentToCanonical("RightShoulder"), "right_shoulder");
  EXPECT_EQ(xmeSegmentToCanonical("LeftHand"), "left_hand");
  EXPECT_EQ(xmeSegmentToCanonical("RightUpperLeg"), "right_upper_leg");
  EXPECT_EQ(xmeSegmentToCanonical("LeftToe"), "left_toe");
}

TEST(XsensModelTest, AllCanonicalMappingsComplete)
{
  const std::vector<std::pair<std::string, std::string>> expected = {
    {"Pelvis", "pelvis"}, {"L5", "l5"}, {"L3", "l3"}, {"T12", "t12"},
    {"T8", "t8"}, {"Sternum", "t8"}, {"Neck", "neck"}, {"Head", "head"},
    {"RightShoulder", "right_shoulder"}, {"RightUpperArm", "right_upper_arm"},
    {"RightForeArm", "right_forearm"}, {"RightHand", "right_hand"},
    {"LeftShoulder", "left_shoulder"}, {"LeftUpperArm", "left_upper_arm"},
    {"LeftForeArm", "left_forearm"}, {"LeftHand", "left_hand"},
    {"RightUpperLeg", "right_upper_leg"}, {"RightLowerLeg", "right_lower_leg"},
    {"RightFoot", "right_foot"}, {"RightToe", "right_toe"},
    {"LeftUpperLeg", "left_upper_leg"}, {"LeftLowerLeg", "left_lower_leg"},
    {"LeftFoot", "left_foot"}, {"LeftToe", "left_toe"},
  };
  for (const auto & [sdk, canonical] : expected) {
    EXPECT_EQ(xmeSegmentToCanonical(sdk), canonical) << "SDK name: " << sdk;
  }
}

TEST(XsensModelTest, UnknownSegmentPassthrough)
{
  EXPECT_EQ(xmeSegmentToCanonical("FutureSensor"), "FutureSensor");
  EXPECT_EQ(xmeSegmentToCanonical(""), "");
}

// ---- Kinetic parent tests ----

TEST(XsensModelTest, KineticParentHasNoRoot)
{
  EXPECT_EQ(kineticParent("pelvis"), "");
}

TEST(XsensModelTest, KineticParentChainNoInfiniteCycle)
{
  XsensModelNames names;
  for (const auto & seg : names.links) {
    std::string cur = seg;
    for (int depth = 0; depth < 25; ++depth) {
      const std::string & p = kineticParent(cur);
      if (p.empty()) {
        break;
      }
      cur = p;
      ASSERT_LT(depth, 24) << "Cycle detected for segment: " << seg;
    }
  }
}

TEST(XsensModelTest, KineticParentUnknownReturnsEmpty)
{
  EXPECT_EQ(kineticParent("not_a_segment"), "");
}

TEST(XsensModelTest, AllLinksExceptSpecialHaveParent)
{
  XsensModelNames names;
  const std::unordered_set<std::string> exempt = {"base_link", "generic_link", "pelvis"};
  for (const auto & link : names.links) {
    if (exempt.count(link)) {
      continue;
    }
    EXPECT_FALSE(kineticParent(link).empty())
      << "Link '" << link << "' has no kinetic parent";
  }
}

TEST(XsensModelTest, AllChainsTerminateAtPelvis)
{
  XsensModelNames names;
  const std::unordered_set<std::string> exempt = {"base_link", "generic_link"};
  for (const auto & seg : names.links) {
    if (exempt.count(seg)) {
      continue;
    }
    std::string cur = seg;
    while (!kineticParent(cur).empty()) {
      cur = kineticParent(cur);
    }
    EXPECT_EQ(cur, "pelvis") << "Chain from '" << seg << "' terminates at '" << cur << "'";
  }
}

// ---- Model structure tests ----

TEST(XsensModelTest, ModelNamesCounts)
{
  XsensModelNames names;
  EXPECT_EQ(names.links.size(), 25u);   // 24 segments + generic_link
  EXPECT_EQ(names.joints.size(), 28u);  // 22 real joints + 6 virtual (_NA) joints
}

TEST(XsensModelTest, SymmetricLeftRightParentChains)
{
  auto chainDepth = [](const std::string & start) {
      int depth = 0;
      std::string cur = start;
      while (!kineticParent(cur).empty()) {
        cur = kineticParent(cur);
        depth++;
      }
      return depth;
    };

  EXPECT_EQ(chainDepth("left_toe"), chainDepth("right_toe"));
  EXPECT_EQ(chainDepth("left_hand"), chainDepth("right_hand"));
  EXPECT_EQ(chainDepth("left_shoulder"), chainDepth("right_shoulder"));
  EXPECT_EQ(chainDepth("left_upper_leg"), chainDepth("right_upper_leg"));
}

TEST(XsensModelTest, NoDuplicateLinkNames)
{
  XsensModelNames names;
  std::unordered_set<std::string> seen;
  for (const auto & link : names.links) {
    EXPECT_TRUE(seen.insert(link).second) << "Duplicate link name: " << link;
  }
}

TEST(XsensModelTest, NoDuplicateJointNames)
{
  XsensModelNames names;
  std::unordered_set<std::string> seen;
  for (const auto & joint : names.joints) {
    EXPECT_TRUE(seen.insert(joint).second) << "Duplicate joint name: " << joint;
  }
}
