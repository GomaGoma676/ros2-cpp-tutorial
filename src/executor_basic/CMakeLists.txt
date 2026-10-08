cmake_minimum_required(VERSION 3.8)
project(executor_basic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(multi_th_executor src/multi_th_executor.cpp)
ament_target_dependencies(multi_th_executor rclcpp)

install(TARGETS
  multi_th_executor
  DESTINATION lib/${PROJECT_NAME}/
)

ament_package()
