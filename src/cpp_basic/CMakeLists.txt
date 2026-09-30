cmake_minimum_required(VERSION 3.8)
project(cpp_basic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
add_executable(basic src/basic.cpp)

install(TARGETS
  basic
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
