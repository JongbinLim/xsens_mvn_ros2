// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <Eigen/Dense>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <xsens_mvn_ros2_common/skeleton_publisher.hpp>

namespace xsens_mvn_ros2
{

/// Abstract interface for a motion capture data source.
/// Both XsensStreamClient (UDP) and future sources implement this interface,
/// enabling unit tests to inject mock data sources without network/hardware.
class IMotionCaptureSource
{
public:
  virtual ~IMotionCaptureSource() = default;

  /// Returns a snapshot of the current segment kinematics, keyed by canonical segment name.
  /// Thread-safe: must be safe to call from a timer callback.
  virtual std::unordered_map<std::string, SegmentKinematics> getSegments() const = 0;

  /// Returns a snapshot of the current joint angles (in radians).
  virtual std::vector<JointAngles> getJoints() const = 0;

  /// Returns the center of mass position in world frame, or nullopt if unavailable.
  virtual std::optional<Eigen::Vector3d> getCOM() const = 0;

  /// Returns nanoseconds (steady clock) of the last data arrival, or 0 if no data yet.
  virtual int64_t lastDataTimeNs() const = 0;

  /// Returns true if the source is actively receiving data.
  virtual bool isActive() const = 0;
};

}  // namespace xsens_mvn_ros2
