// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <algorithm>

namespace xsens_mvn_ros2_description
{

/// Clamp a scale factor to [0.5, 2.0] to prevent degenerate URDF meshes.
inline double clampScale(double scale)
{
  return std::clamp(scale, 0.5, 2.0);
}

/// Compute scale factor for a non-leaf segment.
/// @param measured_distance TF-derived distance between this segment and its child.
/// @param ref_length Reference mesh length.
/// @return Clamped scale factor, or 1.0 if ref_length is non-positive.
inline double computeScale(double measured_distance, double ref_length)
{
  if (ref_length <= 0.0) {
    return 1.0;
  }
  return clampScale(measured_distance / ref_length);
}

}  // namespace xsens_mvn_ros2_description
