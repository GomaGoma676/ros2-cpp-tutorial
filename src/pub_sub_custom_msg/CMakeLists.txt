cmake_minimum_required(VERSION 3.8)
project(pub_sub_custom_msg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_interface REQUIRED)

add_executable(talker_custom src/pub.cpp)
ament_target_dependencies(talker_custom rclcpp custom_interface)
add_executable(listener_custom src/sub.cpp)
ament_target_dependencies(listener_custom rclcpp custom_interface)

install(TARGETS
  talker_custom
  listener_custom
  DESTINATION lib/${PROJECT_NAME})

ament_package()
