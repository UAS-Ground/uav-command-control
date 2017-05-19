// #include <bits/stdc++.h>
#pragma once
#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <hector_uav_msgs/YawrateCommand.h>
#include <hector_uav_msgs/ThrustCommand.h>
#include <hector_uav_msgs/AttitudeCommand.h>
#include <hector_uav_msgs/TakeoffAction.h>
#include <hector_uav_msgs/LandingAction.h>
#include <hector_uav_msgs/PoseAction.h>
#include <hector_quadrotor_interface/limiters.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <hector_uav_msgs/EnableMotors.h>
#include <actionlib/client/simple_action_client.h>
#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <ctime>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>	
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <actionlib/server/simple_action_server.h>
#include <hector_quadrotor_interface/helpers.h>

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "searchrequest.pb.h"
#include "navgoalcommand.pb.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>



using boost::asio::ip::tcp;
using boost::asio::ip::udp;


class ClientROSController {
private:
	boost::shared_ptr<actionlib::SimpleActionClient<hector_uav_msgs::TakeoffAction> > takeoffActionClientPtr;
	boost::shared_ptr<actionlib::SimpleActionClient<hector_uav_msgs::PoseAction> > poseActionClientPtr;
	boost::shared_ptr< ros::NodeHandle > nodeHandlePtr;

	geometry_msgs::PoseStamped currentPose;
	ros::ServiceClient motorEnableServiceClient;
	std::string base_link_frame_, base_stabilized_frame_, world_frame_;
	boost::array<char, 1024> send_buf;
	std::string client_ip_addr;
	std::string client_camera_port;

public:
	boost::shared_ptr< udp::socket > rosCameraSocket;
	udp::endpoint receiver_endpoint;
	std::vector<uchar> buf;
	cv::Mat imageToSend;

	
	ClientROSController();
	ClientROSController(int argc, char ** argv, const std::string ip, const std::string port);

	void start();

	void preparePoseCommand(NavGoalCommand&);
	void debugPoseCommand();
	void prepareMotorService();
	void prepareMotors();

	static void InitalizeSockets(ClientROSController * );

	void imageCb(const sensor_msgs::ImageConstPtr& msg);

	void sendCommand(NavGoalCommand& currentCommand);

};
