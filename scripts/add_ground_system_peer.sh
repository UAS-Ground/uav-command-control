#!/bin/bash

GS_PACKAGE_ROOT=$(pwd)
echo "Set Ground System pkg root to $GS_PACKAGE_ROOT"
mkdir safety
cd safety
SAFETY_DIR=$(pwd)
if [ -d $HOME/hector_quadrotor/src/hector_quadrotor/groundsystem_communication ]
	then
		echo "groundsystem_communication dir already exists"
else	
	echo "groundsystem_communication doesn't exist yet. Creating.."
	cd src/hector_quadrotor
	catkin_create_pkg groundsystem_communication actionlib roscpp sensor_msgs geometry_msgs hector_quadrotor_interface move_base_msgs tf2_geometry_msgs visualization_msgs cv_bridge std_msgs image_transport	
fi
cd $HOME/hector_quadrotor/src/hector_quadrotor/groundsystem_communication
rm -rf *
cp -r $GS_PACKAGE_ROOT/ros-pkg/* $HOME/hector_quadrotor/src/hector_quadrotor/groundsystem_communication
cd $HOME/hector_quadrotor
catkin_make
rm -rf $SAFETY_DIR
cd $GS_PACKAGE_ROOT


