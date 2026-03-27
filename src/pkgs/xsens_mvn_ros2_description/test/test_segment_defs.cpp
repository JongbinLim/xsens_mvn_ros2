// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <array>
#include <string>
#include <unordered_set>

// Pull in the segment table by including the source directly.
// The table is in an anonymous namespace, so we use a header-like approach:
// we re-declare the types and table here to test them without linking the node.

enum class ScaleAxis { X = 0, Y = 1, Z = 2, Uniform = 3 };

struct SegmentDef
{
  const char * name;
  const char * mesh;
  const char * child;
  const char * parent;
  double refLength;
  ScaleAxis scaleAxis;
  ScaleAxis distanceAxis;
  const char * visualRpy;
  const char * visualXyz;
};

static const std::array<SegmentDef, 23> kSegments = {{  // NOLINT
  {"pelvis", "Pelvis", "l5", "world", 0.096, ScaleAxis::Z, ScaleAxis::Uniform, "0 0 0", "0 0 0"},            // NOLINT
  {"l5", "L5", "l3", "pelvis", 0.106, ScaleAxis::Z, ScaleAxis::Uniform, "0 0 0", "0 0 0"},                   // NOLINT
  {"l3", "L3", "t12", "l5", 0.095, ScaleAxis::Z, ScaleAxis::Uniform, "0 0 0", "0 0 0"},                      // NOLINT
  {"t12", "T12", "t8", "l3", 0.094, ScaleAxis::Z, ScaleAxis::Uniform, "0 0 0", "0 0 0"},                     // NOLINT
  {"t8", "T8", "neck", "t12", 0.128, ScaleAxis::Z, ScaleAxis::Uniform, "0 0 0", "0 0 0"},                    // NOLINT
  {"neck", "Neck", "head", "t8", 0.104, ScaleAxis::Z, ScaleAxis::Uniform, "0 0 0", "0 0 0"},                 // NOLINT
  {"head", "Head", "", "neck", 0.0, ScaleAxis::Uniform, ScaleAxis::Uniform, "0 0 0", "0 0 0"},                   // NOLINT
  {"left_shoulder", "LeftShoulder", "left_upper_arm", "t8", 0.140, ScaleAxis::Y, ScaleAxis::Uniform,
    "0 0 0", "0 0 0"},                                                                                                                // NOLINT
  {"left_upper_arm", "LeftUpperArm", "left_forearm", "left_shoulder", 0.306, ScaleAxis::Y,
    ScaleAxis::Uniform, "-1.5708 0 0", "0 0 0"},                                                                                            // NOLINT
  {"left_forearm", "LeftForeArm", "left_hand", "left_upper_arm", 0.254, ScaleAxis::Y,
    ScaleAxis::Uniform, "-1.5708 0 0", "0 0 0"},                                                                                            // NOLINT
  {"left_hand", "LeftHand", "", "left_forearm", 0.0, ScaleAxis::Uniform, ScaleAxis::Uniform,
    "-1.5708 0 0", "0 0 0"},                                                                                                                    // NOLINT
  {"right_shoulder", "RightShoulder", "right_upper_arm", "t8", 0.140, ScaleAxis::Y,
    ScaleAxis::Uniform, "0 0 0", "0 0 0"},                                                                                                // NOLINT
  {"right_upper_arm", "RightUpperArm", "right_forearm", "right_shoulder", 0.306, ScaleAxis::Y,
    ScaleAxis::Uniform, "1.5708 0 0", "0 0 0"},                                                                                                // NOLINT
  {"right_forearm", "RightForeArm", "right_hand", "right_upper_arm", 0.254, ScaleAxis::Y,
    ScaleAxis::Uniform, "1.5708 0 0", "0 0 0"},                                                                                                // NOLINT
  {"right_hand", "RightHand", "", "right_forearm", 0.0, ScaleAxis::Uniform, ScaleAxis::Uniform,
    "1.5708 0 0", "0 0 0"},                                                                                                                        // NOLINT
  {"left_upper_leg", "LeftUpperLeg", "left_lower_leg", "pelvis", 0.417, ScaleAxis::Z,
    ScaleAxis::Uniform, "0 0 0", "0 0 0"},                                                                                            // NOLINT
  {"left_lower_leg", "LeftLowerLeg", "left_foot", "left_upper_leg", 0.408, ScaleAxis::Z,
    ScaleAxis::Uniform, "0 0 0", "0 0 0"},                                                                                            // NOLINT
  {"left_foot", "LeftFoot", "left_toe", "left_lower_leg", 0.1526, ScaleAxis::Uniform, ScaleAxis::X,
    "0 0 0", "0 0 0"},                                                                                                                 // NOLINT
  {"left_toe", "LeftToe", "", "left_foot", 0.0, ScaleAxis::Uniform, ScaleAxis::X, "0 0 0",
    "-0.010 0 -0.015"},                                                                                                                       // NOLINT
  {"right_upper_leg", "RightUpperLeg", "right_lower_leg", "pelvis", 0.417, ScaleAxis::Z,
    ScaleAxis::Uniform, "0 0 0", "0 0 0"},                                                                                               // NOLINT
  {"right_lower_leg", "RightLowerLeg", "right_foot", "right_upper_leg", 0.408, ScaleAxis::Z,
    ScaleAxis::Uniform, "0 0 0", "0 0 0"},                                                                                                // NOLINT
  {"right_foot", "RightFoot", "right_toe", "right_lower_leg", 0.1526, ScaleAxis::Uniform,
    ScaleAxis::X, "0 0 0", "0 0 0"},                                                                                                       // NOLINT
  {"right_toe", "RightToe", "", "right_foot", 0.0, ScaleAxis::Uniform, ScaleAxis::X, "0 0 0",
    "-0.010 0 -0.015"},                                                                                                                           // NOLINT
}};

TEST(SegmentDefs, NoNullParentFields)
{
  for (const auto & seg : kSegments) {
    EXPECT_NE(seg.parent, nullptr) << "Segment " << seg.name << " has null parent";
    EXPECT_NE(std::string(seg.parent), "") << "Segment " << seg.name << " has empty parent "
                                           << "(should be 'world' for root)";
  }
}

TEST(SegmentDefs, NoduplicateNames)
{
  std::unordered_set<std::string> names;
  for (const auto & seg : kSegments) {
    EXPECT_TRUE(names.insert(std::string(seg.name)).second)
      << "Duplicate segment name: " << seg.name;
  }
}

TEST(SegmentDefs, NonLeafRefLengthsPositive)
{
  for (const auto & seg : kSegments) {
    if (std::string(seg.child).empty()) {
      continue;  // leaf
    }
    EXPECT_GT(seg.refLength, 0.0) << "Non-leaf segment " << seg.name << " has refLength <= 0";
  }
}

TEST(SegmentDefs, AllParentsExistOrWorld)
{
  std::unordered_set<std::string> names;
  for (const auto & seg : kSegments) {
    names.insert(std::string(seg.name));
  }
  for (const auto & seg : kSegments) {
    const std::string parent(seg.parent);
    if (parent == "world") {
      continue;
    }
    EXPECT_NE(names.find(parent), names.end())
      << "Segment " << seg.name << " references unknown parent: " << parent;
  }
}

int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
