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

install(TARGETS
  flippers_teleop
  DESTINATION lib/${PROJECT_NAME}
)

# install(DIRECTORY
#   config include
#   DESTINATION share/${PROJECT_NAME}
# )

ament_package()
