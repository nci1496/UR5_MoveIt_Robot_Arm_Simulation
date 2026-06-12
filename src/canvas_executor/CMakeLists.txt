cmake_minimum_required(VERSION 3.0.2)
project(canvas_executor)

## Find catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  moveit_ros_planning_interface
  moveit_ros_planning
  pluginlib
  moveit_kinematics
)

################################################
## catkin specific configuration ##
################################################
catkin_package(
  CATKIN_DEPENDS roscpp std_msgs geometry_msgs
)

###################################
## Build ##
###################################

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare C++ executable
add_executable(canvas_executor
    src/canvas_executor.cpp
)

## Link libraries
target_link_libraries(canvas_executor
    ${catkin_LIBRARIES}
)
