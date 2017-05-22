#!/bin/bash

sudo echo "Root permission obtained..."
UAV_ROOT=$(pwd)
cd $HOME
if [ -d $HOME/hector_quadrotor ]
	then
		echo "hector_quadrotor already on disk"
else
		git clone https://github.com/tu-darmstadt-ros-pkg/hector_quadrotor.git
fi
cd hector_quadrotor
HECTOR_ROOT=$(pwd)
if [ -d src ]
	then
		echo "hector_quadrotor already initialized"
else
		git checkout kinetic-devel
		wstool init src tutorials.rosinstall

fi
cd src/hector_quadrotor
if [ -d groundsystem_communication ]
	then
		echo "groundsystem_communication already created"
else
		catkin_create_pkg groundsystem_communication actionlib roscpp sensor_msgs geome$
fi
cd $UAV_ROOT
./scripts/add_ground_system_peer.sh
echo "Hector Quadrotor build complete."
source $HOME/hector_quadrotor/devel/setup.bash
echo "source $HECTOR_ROOT/devel/setup.bash" >> ~/.bashrc
echo "export HECTOR_ROOT=$HECTOR_ROOT" >> ~/.bashrc
echo "export UAV_ROOT=$UAV_ROOT" >> ~/.bashrc


