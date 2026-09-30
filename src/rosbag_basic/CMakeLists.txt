cmake_minimum_required(VERSION 3.8)
project(rosbag_basic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosbag2_cpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(rosbag_recorder src/recorder.cpp)
ament_target_dependencies(rosbag_recorder rclcpp rosbag2_cpp std_msgs)

install(TARGETS
  rosbag_recorder
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
