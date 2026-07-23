ARG ROS_DISTRO=jazzy
FROM ros:${ROS_DISTRO}-ros-base

ARG ROS_DISTRO
ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_DISTRO=${ROS_DISTRO}
ENV XSENS_WS=/opt/xsens_ws

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    python3-colcon-common-extensions \
    python3-rosdep \
    && rm -rf /var/lib/apt/lists/*

WORKDIR ${XSENS_WS}
COPY src/ src/
COPY colcon_defaults.yaml ./

RUN rosdep update \
    && apt-get update \
    && rosdep install --from-paths src --ignore-src --rosdistro ${ROS_DISTRO} -r -y \
    && rm -rf /var/lib/apt/lists/*

# Override the repository's debug/coverage defaults for the runtime image.
RUN source /opt/ros/${ROS_DISTRO}/setup.bash \
    && colcon build \
      --merge-install \
      --cmake-args \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_FLAGS=-O2

COPY docker/ros_entrypoint.sh /ros_entrypoint.sh
RUN chmod +x /ros_entrypoint.sh \
    && mkdir -p /var/lib/xsens /var/log/xsens

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
