
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
#include "CommandTCPConnection.h"
#include "ClientROSController.h"



using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class CommandTCPServer
{
public:
	CommandTCPServer(boost::asio::io_service& io_service, ClientROSController * clientROSController);

private:

	ClientROSController * clientROSController;

	void start_accept();

	void handle_accept(boost::shared_ptr<CommandTCPConnection> new_connection, const boost::system::error_code& error);

	tcp::acceptor acceptor_;
};