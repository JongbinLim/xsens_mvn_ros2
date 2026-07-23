#!/usr/bin/env bash
set -e

source "/opt/ros/${ROS_DISTRO}/setup.bash"
source "${XSENS_WS}/install/setup.bash"

mkdir -p /var/lib/xsens /var/log/xsens

exec "$@"
