cmake_minimum_required(VERSION 3.8)
project(xsens_mvn_sdk)

find_package(ament_cmake REQUIRED)
find_package(Eigen3 REQUIRED)

add_library(${PROJECT_NAME} SHARED
  src/angularsegmentkinematicsdatagram.cpp
  src/centerofmassdatagram.cpp
  src/datagram.cpp
  src/eulerdatagram.cpp
  src/jointanglesdatagram.cpp
  src/linearsegmentkinematicsdatagram.cpp
  src/metadatagram.cpp
  src/parsermanager.cpp
  src/positiondatagram.cpp
  src/quaterniondatagram.cpp
  src/scaledatagram.cpp
  src/streamer.cpp
  src/timecodedatagram.cpp
  src/trackerkinematicsdatagram.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(Eigen3)
ament_package()
