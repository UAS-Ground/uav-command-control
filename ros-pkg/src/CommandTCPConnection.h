#pragma once
#include <boost/algorithm/string/predicate.hpp>
#include <ctime>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "navgoalcommand.pb.h"
#include "ClientROSController.h"


using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class CommandTCPConnection : public boost::enable_shared_from_this<CommandTCPConnection>
{
public:
	NavGoalCommand currentCommand;
	ClientROSController * clientROSController;

	static boost::shared_ptr<CommandTCPConnection> create(boost::asio::io_service& io_service);

	tcp::socket& socket();

	void setClientROSController(ClientROSController * clientROSController);
	void SendCommandToUAV();

	void handle_read_content(const boost::system::error_code& err);

	void start();

private:
	CommandTCPConnection(boost::asio::io_service& io_service);

	void handle_write();

	boost::asio::streambuf response_;

	tcp::socket socket_;
	std::string message_;

};
