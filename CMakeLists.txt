cmake_minimum_required(VERSION 3.0.2)
project(mower_robot)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  tf
  serial
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES mower_robot
#  CATKIN_DEPENDS roscpp rospy std_msgs tf
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(mower_robot_joy src/mower_robot_joy.cpp)
add_executable(mower_robot_serial src/mower_robot_serial.cpp)
#add_executable(mower_robot_serial_class src/mower_robot_serial_class.cpp)
add_executable(mower_robot_serial_test src/mower_robot_serial_test.cpp)
add_executable(mower_robot_plotter src/mower_robot_plotter.cpp)
add_executable(mower_robot_barikan src/mower_robot_barikan.cpp)

target_link_libraries(mower_robot_joy
  ${catkin_LIBRARIES}
)
target_link_libraries(mower_robot_serial
  ${catkin_LIBRARIES}
)

target_link_libraries(mower_robot_serial_test
  ${catkin_LIBRARIES}
)

target_link_libraries(mower_robot_plotter
  ${catkin_LIBRARIES}
)

target_link_libraries(mower_robot_barikan
  ${catkin_LIBRARIES}
)

catkin_install_python(PROGRAMS
  scripts/mower_robot_serial.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
