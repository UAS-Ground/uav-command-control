cmake_minimum_required(VERSION 2.8.3)
project(groundsystem_communication)





find_package(catkin REQUIRED COMPONENTS
  actionlib
  roscpp
  sensor_msgs
  geometry_msgs
  hector_quadrotor_interface
  move_base_msgs
  tf2_geometry_msgs
  visualization_msgs 
  cv_bridge 
  std_msgs 
  image_transport)


find_package(OpenCV 3.1.0 REQUIRED COMPONENTS core highgui imgproc imgcodecs)
include_directories(include ${catkin_INCLUDE_DIRS})
catkin_package(
    INCLUDE_DIRS
    LIBRARIES
    CATKIN_DEPENDS roscpp sensor_msgs geometry_msgs hector_uav_msgs cv_bridge std_msgs image_transport
    DEPENDS
)

add_executable(groundsystem_communication_node src/uas_server.cpp src/searchrequest.pb.cc src/navgoalcommand.pb.cc src/CommandTCPConnection.cpp src/CommandTCPServer.cpp src/ClientROSController.cpp)

link_directories(/usr/local/lib)

target_link_libraries(groundsystem_communication_node protobuf opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)


target_link_libraries(groundsystem_communication_node ${catkin_LIBRARIES})
add_dependencies(groundsystem_communication_node ${catkin_EXPORTED_TARGETS})

install(TARGETS groundsystem_communication_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch
  DESTINATION  ${CATKIN_PACKAGE_SHARE_DESTINATION}
)


