#include "CommandTCPServer.h"
#include "ClientROSController.h"
#define DEBUG_UAV false
#define DEBUG_CMD true
#define FRAME_HEIGHT 240
#define FRAME_WIDTH 320
#define FRAME_INTERVAL (1000/30)
#define CAM_FRAME_UDP_PKG_SIZE 4096
#define ENCODE_QUALITY 80
#define TIME_AWAIT_BOOT 7

// helper class to connect subscriber callback to transportation logic
class CameraSubscriberHandler
{
private:
	ClientROSController * clientROSController;
public:
	CameraSubscriberHandler(ClientROSController * clientROSController) : clientROSController(clientROSController)
	{
	}
	void compressImage(cv_bridge::CvImagePtr& cv_ptr)
	{	
	    cv::resize(cv_ptr->image, clientROSController->imageToSend, cv::Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, cv::INTER_LINEAR);
	    std::vector < int > compression_params;
	    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	    compression_params.push_back(ENCODE_QUALITY);
	    cv::imencode(".jpg", clientROSController->imageToSend, clientROSController->buf, compression_params);
	}
	void printMsgInfo(const sensor_msgs::ImageConstPtr& msg)
	{
		std::cout << "\t in imageDb with a msg!\n";
		std::cout << "\t----- msg->height " << msg->height << "\n";
		std::cout << "\t----- msg->width " << msg->width << "\n";
		std::cout << "\t----- msg->encoding " << msg->encoding << "\n";
		std::cout << "\t----- msg->is_bigendian " << msg->is_bigendian << "\n";
		std::cout << "\t----- msg->step " << msg->step << "\n";
	}
	void printCVImageInfo(cv_bridge::CvImagePtr& cv_ptr)
	{
		// interrogate cv_ptr->image, which is a cv::Mat object defined by OpenCV
		std::cout << "\t----- cv_ptr->image.rows " << cv_ptr->image.rows << "\n";
		std::cout << "\t----- cv_ptr->image.cols " << cv_ptr->image.cols << "\n";
	}
	void convertROSMessageToCV(const sensor_msgs::ImageConstPtr& msg, cv_bridge::CvImagePtr& cv_ptr)
	{
		try
		{
		  cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		}
		catch (cv_bridge::Exception& e)
		{
		  ROS_ERROR("cv_bridge exception: %s", e.what());
		  return;
		}
	}
	void sendImage()
	{
		// calculate how many UDP packets this frame needs
		int packetCount = 1 + (clientROSController->buf.size() - 1) / CAM_FRAME_UDP_PKG_SIZE;
		// send the packet count
	    int packetCountBuffer[1];
	    packetCountBuffer[0] = packetCount;
		clientROSController->rosCameraSocket->send_to(boost::asio::buffer(packetCountBuffer), clientROSController->receiver_endpoint);
		// send the packets
	    for (int i = 0; i < packetCount; i++)
	    {
			clientROSController->rosCameraSocket->send_to(boost::asio::buffer(&(clientROSController->buf)[i * CAM_FRAME_UDP_PKG_SIZE], CAM_FRAME_UDP_PKG_SIZE), clientROSController->receiver_endpoint);
	    }
	}
	void imageCb(const sensor_msgs::ImageConstPtr& msg)
	{
		if(DEBUG_UAV)
		{
			printMsgInfo(msg);
		}
		cv_bridge::CvImagePtr cv_ptr;
		convertROSMessageToCV(msg, cv_ptr);
		compressImage(cv_ptr);
		sendImage();
		if(DEBUG_UAV)
		{
			printCVImageInfo(cv_ptr);
		}
	}
};

ClientROSController::ClientROSController()
{
}

ClientROSController::ClientROSController(int argc, char ** argv, const std::string ip, const std::string port)
{
	ros::init(argc, argv, "simple_navigation_goals_node");
	client_ip_addr = ip;
	client_camera_port = port;
  	nodeHandlePtr = boost::shared_ptr< ros::NodeHandle >(new ros::NodeHandle());
    nodeHandlePtr->param<std::string>("base_link_frame", base_link_frame_, "base_link");
    nodeHandlePtr->param<std::string>("world_frame", world_frame_, "world");
    nodeHandlePtr->param<std::string>("base_stabilized_frame", base_stabilized_frame_, "base_stabilized");
  	poseActionClientPtr = boost::shared_ptr<actionlib::SimpleActionClient<hector_uav_msgs::PoseAction> >(new actionlib::SimpleActionClient<hector_uav_msgs::PoseAction>(*nodeHandlePtr, "action/pose"));
  	takeoffActionClientPtr = boost::shared_ptr<actionlib::SimpleActionClient<hector_uav_msgs::TakeoffAction> >(new actionlib::SimpleActionClient<hector_uav_msgs::TakeoffAction>(*nodeHandlePtr, "action/takeoff"));
  	prepareMotorService();
}

void ClientROSController::start()
{
  	boost::thread server_thread(&(this->InitalizeSockets), this);
	boost::this_thread::sleep_for( boost::chrono::seconds(TIME_AWAIT_BOOT) );
	//ros::Subscriber imageSubscriber = nodeHandlePtr->subscribe("/front_cam/camera/image", 1, imageCb, this);
	CameraSubscriberHandler subscribeHandler(this);
	ros::Subscriber imageSubscriber = nodeHandlePtr->subscribe("/front_cam/camera/image", 1, &CameraSubscriberHandler::imageCb, &subscribeHandler);
	std::cout << "i have subscribed to /front_cam/camera/image\n";
	std::cout << "\n********\nin main(), about to ros::spin()\n********\n";
  	ros::spin();
  	server_thread.join();
}

void ClientROSController::InitalizeSockets(ClientROSController * rosController)
{
	std::cout << "i am in InitalizeSockets()\n";
	try
	{
		boost::asio::io_service io_service;
		CommandTCPServer server1(io_service, rosController);	
		try
		{
			udp::resolver resolver(io_service);
			udp::resolver::query query(udp::v4(), rosController->client_ip_addr, rosController->client_camera_port);
			rosController->receiver_endpoint = *resolver.resolve(query);
			
			rosController->rosCameraSocket = boost::shared_ptr< udp::socket >(new udp::socket(io_service));
			rosController->rosCameraSocket->open(udp::v4());
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		io_service.run();
		std::cout << "i am in main(), called io_service.run()...\n";
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void ClientROSController::imageCb(const sensor_msgs::ImageConstPtr& msg)
{
	std::cout << "\t in imageDb with a msg!\n";
	std::cout << "\t----- msg->height " << msg->height << "\n";
	std::cout << "\t----- msg->width " << msg->width << "\n";
	std::cout << "\t----- msg->encoding " << msg->encoding << "\n";
	std::cout << "\t----- msg->is_bigendian " << msg->is_bigendian << "\n";
	std::cout << "\t----- msg->step " << msg->step << "\n";
	cv_bridge::CvImagePtr cv_ptr;
	try
	{
	  cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception& e)
	{
	  ROS_ERROR("cv_bridge exception: %s", e.what());
	  return;
	}
	std::cout << "\t Converted to cvImage!\n";
	std::cout << "\t----- cv_ptr->image.rows " << cv_ptr->image.rows << "\n";
	std::cout << "\t----- cv_ptr->image.cols " << cv_ptr->image.cols << "\n";
    cv::resize(cv_ptr->image, imageToSend, cv::Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, cv::INTER_LINEAR);
    std::vector < int > compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(ENCODE_QUALITY);
    cv::imencode(".jpg", imageToSend, buf, compression_params);
	int packetCount = 1 + (buf.size() - 1) / CAM_FRAME_UDP_PKG_SIZE;
    int packetCountBuffer[1];
    packetCountBuffer[0] = packetCount;
	rosCameraSocket->send_to(boost::asio::buffer(packetCountBuffer), receiver_endpoint);
    for (int i = 0; i < packetCount; i++)
    {
    	//& buf[i * CAM_FRAME_UDP_PKG_SIZE], CAM_FRAME_UDP_PKG_SIZE
		rosCameraSocket->send_to(boost::asio::buffer(&buf[i * CAM_FRAME_UDP_PKG_SIZE], CAM_FRAME_UDP_PKG_SIZE), receiver_endpoint);
    }
}


void ClientROSController::debugPoseCommand()
{
	std::cout << "Sending: pose command:\n";
	std::cout << "	x: " << currentPose.pose.position.x << "\n";
	std::cout << "	y: " << currentPose.pose.position.y << "\n";
	std::cout << "	z: " << currentPose.pose.position.z << "\n";

}

void ClientROSController::prepareMotorService()
{
	
	motorEnableServiceClient = nodeHandlePtr->serviceClient<hector_uav_msgs::EnableMotors>("enable_motors");
    if (!motorEnableServiceClient.waitForExistence(ros::Duration(5.0)))
    {
      ROS_WARN("Motor enable service not found");
      return;
    }
    else
    {
    	std::cout << "*** Found motor enable service!\n";
    }
}

void ClientROSController::prepareMotors()
{

    hector_uav_msgs::EnableMotors srv;
    srv.request.enable = true;
    if(motorEnableServiceClient.call(srv))
    {
    	std::cout << "*** Launched motor enable service with srv.request.enable=true!\n";
    }
    else
    {
      ROS_WARN("Could not motor enable service with srv.request.enable=true");
      return;
    }
	
}

void ClientROSController::preparePoseCommand(NavGoalCommand& currentCommand)
{
	std::cout << "\t Successful parse\n";
	std::cout << "\t\t Successful parse\n";
	std::cout << "\t\t currentCommand.x() " << currentCommand.x() << "\n";
	std::cout << "\t\t currentCommand.y() " << currentCommand.y() << "\n";
	std::cout << "\t\t currentCommand.z() " << currentCommand.z() << "\n";
	double current_x = currentCommand.x();
	double current_y = currentCommand.y();
	double current_z = currentCommand.z();
	ros::Time now = ros::Time::now();
	double dt = 0.0;
	if (!currentPose.header.stamp.isZero()) {
		dt = std::max(0.0, std::min(1.0, (now - currentPose.header.stamp).toSec()));
	}
	currentPose.header.stamp = now;
	currentPose.header.frame_id = world_frame_;
	currentPose.pose.position.x = current_x;
	currentPose.pose.position.y = current_y;
	currentPose.pose.position.z = current_z;

	if(DEBUG_UAV || DEBUG_CMD)
	{
		debugPoseCommand();
	}

}

void ClientROSController::sendCommand(NavGoalCommand& currentCommand)
{
	prepareMotors();
    preparePoseCommand(currentCommand);

	hector_uav_msgs::PoseGoal goal;
	goal.target_pose = currentPose;
	poseActionClientPtr->sendGoal(goal);
}
