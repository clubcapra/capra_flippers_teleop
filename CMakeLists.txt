cmake_minimum_required(VERSION 3.8)
project(capra_flippers_teleop)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(THIS_PROJECT_DEPEND
  rclcpp
  capra_control_msgs
  sensor_msgs
)

# find dependencies
find_package(ament_cmake REQUIRED)
foreach(dep IN ITEMS ${THIS_PROJECT_DEPEND})
  find_package(${dep} REQUIRED)
endforeach(dep)

add_executable(flippers_teleop src/flippers_teleop_node.cpp)
ament_target_dependencies(flippers_teleop
  ${THIS_PROJECT_DEPEND}
)
target_compile_features(flippers_teleop PRIVATE cxx_std_20)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
  flippers_teleop
  DESTINATION lib/${PROJECT_NAME}
)

# install(DIRECTORY
#   config include
#   DESTINATION share/${PROJECT_NAME}
# )

ament_package()
