// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include <gtest/gtest.h>
#include "xsens_mvn_ros2_description/scale_utils.hpp"

using xsens_mvn_ros2_description::clampScale;
using xsens_mvn_ros2_description::computeScale;

// ---- clampScale tests ----

TEST(ScaleUtils, ClampWithinRange)
{
  EXPECT_DOUBLE_EQ(clampScale(1.0), 1.0);
  EXPECT_DOUBLE_EQ(clampScale(1.5), 1.5);
  EXPECT_DOUBLE_EQ(clampScale(0.5), 0.5);
  EXPECT_DOUBLE_EQ(clampScale(2.0), 2.0);
}

TEST(ScaleUtils, ClampBelowMinimum)
{
  EXPECT_DOUBLE_EQ(clampScale(0.1), 0.5);
  EXPECT_DOUBLE_EQ(clampScale(-1.0), 0.5);
  EXPECT_DOUBLE_EQ(clampScale(0.0), 0.5);
}

TEST(ScaleUtils, ClampAboveMaximum)
{
  EXPECT_DOUBLE_EQ(clampScale(2.5), 2.0);
  EXPECT_DOUBLE_EQ(clampScale(100.0), 2.0);
}

TEST(ScaleUtils, ClampBoundaryValues)
{
  EXPECT_DOUBLE_EQ(clampScale(0.49999), 0.5);
  EXPECT_DOUBLE_EQ(clampScale(2.00001), 2.0);
}

// ---- computeScale tests ----

TEST(ScaleUtils, ComputeScaleNormal)
{
  EXPECT_DOUBLE_EQ(computeScale(0.2, 0.1), 2.0);
  EXPECT_DOUBLE_EQ(computeScale(0.1, 0.1), 1.0);
  EXPECT_DOUBLE_EQ(computeScale(0.15, 0.1), 1.5);
}

TEST(ScaleUtils, ComputeScaleClampedHigh)
{
  EXPECT_DOUBLE_EQ(computeScale(1.0, 0.1), 2.0);
  EXPECT_DOUBLE_EQ(computeScale(5.0, 0.1), 2.0);
}

TEST(ScaleUtils, ComputeScaleClampedLow)
{
  EXPECT_DOUBLE_EQ(computeScale(0.01, 0.1), 0.5);
  EXPECT_DOUBLE_EQ(computeScale(0.0, 0.1), 0.5);
}

TEST(ScaleUtils, ComputeScaleZeroRefLength)
{
  EXPECT_DOUBLE_EQ(computeScale(0.5, 0.0), 1.0);
  EXPECT_DOUBLE_EQ(computeScale(0.5, -0.1), 1.0);
}

int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
