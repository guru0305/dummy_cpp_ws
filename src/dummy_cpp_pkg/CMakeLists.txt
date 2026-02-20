cmake_minimum_required(VERSION 3.8)
project(dummy_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies first
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# Library
add_library(math_utils src/math_utils.cpp)
target_include_directories(math_utils PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# Executable
add_executable(dummy_node src/dummy_node.cpp)
ament_target_dependencies(dummy_node rclcpp)

# Tests
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)

  ament_add_gtest(test_math test/test_math.cpp)
  target_link_libraries(test_math math_utils)
endif()

# Install
install(TARGETS
  math_utils
  dummy_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include/)

ament_package()
