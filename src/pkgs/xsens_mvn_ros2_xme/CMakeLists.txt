cmake_minimum_required(VERSION 3.8)
project(xsens_mvn_ros2_xme)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_link_options(-Wl,--no-undefined)

find_package(ament_cmake REQUIRED)
find_package(diagnostic_msgs REQUIRED)
find_package(diagnostic_updater REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(generate_parameter_library REQUIRED)
find_package(magic_enum REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(xsens_mvn_msgs REQUIRED)
find_package(xsens_mvn_ros2_common REQUIRED)
find_package(xsens_xme_sdk REQUIRED)

generate_parameter_library(xsens_xme_node_parameters
  src/xsens_xme_node_parameters.yaml
)

add_executable(xsens_mvn_ros2_xme_node
  src/xsens_xme_node.cpp
  src/diagnostics.cpp
  src/calibration.cpp
  src/configuration.cpp
)
target_include_directories(xsens_mvn_ros2_xme_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(xsens_mvn_ros2_xme_node
  xsens_mvn_ros2_common::xsens_mvn_ros2_common_SkeletonPublisher
  xsens_xme_node_parameters
  xsens_mvn_ros2_common::body_dimensions_parameters
  ${diagnostic_msgs_TARGETS}
  ${lifecycle_msgs_TARGETS}
  ${xsens_mvn_msgs_TARGETS}
  diagnostic_updater::diagnostic_updater
  Eigen3::Eigen
  ${tf2_geometry_msgs_TARGETS}
  rclcpp::rclcpp
  rclcpp_action::rclcpp_action
  rclcpp_lifecycle::rclcpp_lifecycle
  tf2_ros::tf2_ros
  xsens_xme_sdk::xsens_xme_sdk
  magic_enum::magic_enum
)

install(TARGETS xsens_mvn_ros2_xme_node
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
  # Hardware-in-loop tests only: no automated unit tests for XME node.
  # The XME SDK requires physical hardware (Awinda station + suit) to initialize.
endif()

ament_package()
