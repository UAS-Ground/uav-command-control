// #include <bits/stdc++.h>
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
#include "CommandTCPServer.h"
#include "ClientROSController.h"

static const std::string OPENCV_WINDOW = "Image window";


#define CLIENT_IP "localhost"
#define CLIENT_CAMERA_PORT "8080"




/*

void cameraSocketWriteHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	std::cout << "in cameraSocketWriteHandler..\n";
	std::cout << "--- sent " << bytes_transferred << " bytes..\n";
	if(!ec)
	{
		std::cout << "No errors...\n";

	}
	else if (ec != boost::asio::error::eof)
	{
		std::cout << "Error: " << ec << ": " << ec.message() << "\n";
	}
	else
	{
		std::cout << "Got to the EOF\n";

	}
}

*/
/*
void compressedImageCb(const sensor_msgs::CompressedImageConstPtr& msg)
{
	std::cout << "\t in compressedImageCb with a msg!\n";
	std::cout << "\t---- size: " << sizeof(msg->data) << "\n";
	std::cout << "\t---- format: " << msg->format << "\n";
	std::cout << "\t---- data: \n";


	for(int i = 0; i < msg->data.size() && i < 50; i++){
		std::cout << " " << msg->data[i];
	}

	std::cout << "\nJust made buffer vector<uchar>.... About to send to :\n";
	std::cout << "\t---- IP:   " << receiver_endpoint.address().to_string() << "\n";
	std::cout << "\t---- Port: " << receiver_endpoint.port() << "\n";
	rosCameraSocket->async_send_to(boost::asio::buffer(msg->data), receiver_endpoint, cameraSocketWriteHandler);
	std::cout << "\tBack from call to async_send_to()...\n";



}
*/
int main(int argc, char ** argv){

  	GOOGLE_PROTOBUF_VERIFY_VERSION;

  	ClientROSController clientROSController(argc, argv, CLIENT_IP, CLIENT_CAMERA_PORT);
  	clientROSController.start();

  	//camera_thread.join();
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}