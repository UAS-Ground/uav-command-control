# UAV Server Node

This repository contains a custom ROS package for the UAV Ground System project at CSULA. This implementation has been adapted to integrate with the [hector_quadrotor](https://github.com/tu-darmstadt-ros-pkg/hector_quadrotor.git) project. The installation script in this repository will download and configure hector_quadrotor with the Ground System server package.

## Installation

1. Clone the repo
```bash
git clone https://github.com/UAS-Ground/uav-command-control.git
```
2. Run the install script
```bash
cd uav-command-control
./install.sh
```

3. See the [client project repository](https://github.com/UAS-Ground/front) for details about building and running this project