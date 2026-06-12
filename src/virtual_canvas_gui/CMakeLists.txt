cmake_minimum_required(VERSION 3.0.2)
project(virtual_canvas_gui)

## Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

## Find catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
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

## Generate moc files
qt5_wrap_cpp(canvas_widget_MOCS include/canvas_widget.h)
qt5_wrap_cpp(main_window_MOCS include/main_window.h)

## Declare C++ executable
add_executable(virtual_canvas_gui
    src/main.cpp
    src/main_window.cpp
    src/canvas_widget.cpp
    ${canvas_widget_MOCS}
    ${main_window_MOCS}
)

## Add Qt5 and catkin libraries
target_link_libraries(virtual_canvas_gui
    Qt5::Widgets
    ${catkin_LIBRARIES}
)
