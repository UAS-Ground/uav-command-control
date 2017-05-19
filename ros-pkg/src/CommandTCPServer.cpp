#include "CommandTCPServer.h"

CommandTCPServer::CommandTCPServer(boost::asio::io_service& io_service, ClientROSController * clientROSController)	: acceptor_(io_service, tcp::endpoint(tcp::v4(), 5555))
{
	this->clientROSController = clientROSController;
	std::cout << "i am in the public constructor for CommandTCPServer()\n";
	start_accept();
}



void CommandTCPServer::start_accept()
{
	std::cout << "i am in the private method for CommandTCPServer::start_accept()\n";
	boost::shared_ptr<CommandTCPConnection> new_connection = CommandTCPConnection::create(acceptor_.get_io_service());
	new_connection->setClientROSController(this->clientROSController);

	acceptor_.async_accept(new_connection->socket(), boost::bind(&CommandTCPServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void CommandTCPServer::handle_accept(boost::shared_ptr<CommandTCPConnection> new_connection, const boost::system::error_code& error)
{
	std::cout << "i am in the private method for CommandTCPServer::handle_accept()\n";
	if (!error)
	{
		std::cout << "received a new connection from " << new_connection->socket().remote_endpoint().address().to_string() << ":" << new_connection->socket().remote_endpoint().port() << "\n";
		new_connection->start();
	}

	start_accept();
}
