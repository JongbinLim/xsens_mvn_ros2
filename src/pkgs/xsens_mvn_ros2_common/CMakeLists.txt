cmake_minimum_required(VERSION 3.8)
project(xsens_mvn_ros2_common)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Eigen3 REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(generate_parameter_library REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_eigen REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(xsens_mvn_msgs REQUIRED)

generate_parameter_library(body_dimensions_parameters
  src/body_dimensions_parameters.yaml
)

add_library(${PROJECT_NAME}_SkeletonPublisher SHARED src/skeleton_publisher.cpp)
target_include_directories(${PROJECT_NAME}_SkeletonPublisher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)
target_link_libraries(${PROJECT_NAME}_SkeletonPublisher
  ${geometry_msgs_TARGETS}
  ${sensor_msgs_TARGETS}
  ${xsens_mvn_msgs_TARGETS}
  Eigen3::Eigen
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
  tf2_eigen::tf2_eigen
  tf2_ros::tf2_ros
)

install(TARGETS ${PROJECT_NAME}_SkeletonPublisher body_dimensions_parameters
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME})

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(
  Eigen3 generate_parameter_library geometry_msgs rclcpp rclcpp_lifecycle
  sensor_msgs tf2_eigen tf2_ros xsens_mvn_msgs
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()

  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_xsens_model test/test_xsens_model.cpp)
  target_include_directories(test_xsens_model PRIVATE include)

  ament_add_gtest(test_skeleton_publisher test/test_skeleton_publisher.cpp)
  target_link_libraries(test_skeleton_publisher ${PROJECT_NAME}_SkeletonPublisher)

  ament_add_gtest(test_skeleton_publisher_node test/test_skeleton_publisher_node.cpp)
  target_link_libraries(test_skeleton_publisher_node
    ${PROJECT_NAME}_SkeletonPublisher
    ${xsens_mvn_msgs_TARGETS}
    rclcpp::rclcpp
    rclcpp_lifecycle::rclcpp_lifecycle
    tf2_ros::tf2_ros
    tf2_eigen::tf2_eigen
  )
endif()

ament_package()
