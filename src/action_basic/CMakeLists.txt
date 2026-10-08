cmake_minimum_required(VERSION 3.8)
project(action_basic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(custom_action_interfaces REQUIRED)

add_executable(increment_server src/increment_action_server.cpp)
ament_target_dependencies(increment_server rclcpp rclcpp_action custom_action_interfaces)

install(TARGETS
  increment_server
  DESTINATION lib/${PROJECT_NAME}/
)

ament_package()
