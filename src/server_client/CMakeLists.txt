cmake_minimum_required(VERSION 3.8)
project(service_client)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)

add_executable(add_server src/server.cpp)
ament_target_dependencies(add_server rclcpp example_interfaces)
add_executable(add_client src/client.cpp)
ament_target_dependencies(add_client rclcpp example_interfaces)

install(TARGETS
  add_server
  add_client
  DESTINATION lib/${PROJECT_NAME})

  ament_package()
