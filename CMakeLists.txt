cmake_minimum_required(VERSION 2.8.3)
project(ros_ffmpeg_controller)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED
  roscpp
  cv_bridge
  image_transport
  OpenCV
  sensor_msgs
  message_generation
)

add_service_files(
  FILES
  ExecuteCommand.srv
)


generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} 
    src/command_executor.cpp
)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})


add_executable(ffmpeg_controller src/ros_ffmpeg_controller.cpp)
add_dependencies(ffmpeg_controller ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS} ros_ffmpeg_controller_generate_messages_cpp)
target_link_libraries(ffmpeg_controller ${PROJECT_NAME}
                             ${catkin_LIBRARIES}
)
