#!/bin/bash

sudo echo "Root permission obtained..."
UAV_ROOT=$(pwd)
cd $HOME
git clone https://github.com/tu-darmstadt-ros-pkg/hector_quadrotor.git
cd hector_quadrotor
git checkout kinetic-devel
wstool init src tutorials.rosinstall
cd src/hector_quadrotor
catkin_create_pkg groundsystem_communication actionlib roscpp sensor_msgs geometry_msgs hector_quadrotor_interface move_base_msgs tf2_geometry_msgs visualization_msgs cv_bridge std_msgs image_transport
./scripts/add_ground_system_peer.sh
echo "Hector Quadrotor build complete."
HECTOR_ROOT=$(pwd)
echo "source $HECTOR_ROOT/devel/setup.bash" >> ~/.bashrc
echo "export HECTOR_ROOT=$HECTOR_ROOT" >> ~/.bashrc
echo "export UAV_ROOT=$UAV_ROOT" >> ~/.bashrc


