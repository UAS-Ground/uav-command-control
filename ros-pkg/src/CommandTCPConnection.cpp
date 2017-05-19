#include "CommandTCPConnection.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

boost::shared_ptr<CommandTCPConnection> CommandTCPConnection::CommandTCPConnection::create(boost::asio::io_service& io_service)
{
	std::cout << "i am in the public static method CommandTCPConnection::create()....\n";
	return boost::shared_ptr<CommandTCPConnection>(new CommandTCPConnection(io_service));
}

tcp::socket& CommandTCPConnection::socket()
{
	std::cout << "i am in the public method CommandTCPConnection::socket()....\n";
	return socket_;
}

void CommandTCPConnection::setClientROSController(ClientROSController * clientROSController)
{
	this->clientROSController = clientROSController;
}

void CommandTCPConnection::SendCommandToUAV()
{
	this->clientROSController->sendCommand(this->currentCommand);
}

void CommandTCPConnection::handle_read_content(const boost::system::error_code& err)
{
	std::cout << "i am in the public method CommandTCPConnection::handle_read_content()....\n";
	if (!err)
	{
		currentCommand.Clear();

  		// Write all of the data that has been read so far.

		boost::asio::streambuf::const_buffers_type bufs = response_.data();
		std::string str(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response_.size());

		std::cout << "\t***RAW RESPONSE WAS " <<  &response_ << ", about to parse it\n";
		std::cout << "\t***str RESPONSE WAS " <<  str << ", about to parse it\n";
  		if(!currentCommand.ParseFromString(str))
  		{
			std::cout << "\tFailed to parse\n";

  		} 
  		else
  		{


  			SendCommandToUAV();


  		}

		std::cout << "i am in the public method CommandTCPConnection::handle_read_content(), about to call async_read()....\n";
  		// Continue reading remaining data until EOF.
		boost::asio::async_read(socket_, response_, boost::asio::transfer_at_least(1), boost::bind(&CommandTCPConnection::handle_read_content, this, boost::asio::placeholders::error));
	}
	else if (err != boost::asio::error::eof)
	{
		std::cout << "Error: " << err << ": " << err.message() << "\n";
	}
	else
	{
		std::cout << "Got to the EOF\n";

	}
}

void CommandTCPConnection::start()
{
	std::cout << "i am in the public method CommandTCPConnection::start()....\n";


	boost::array<char, 128> buf;
	boost::system::error_code error;

		//size_t len = socket.read_some(boost::asio::buffer(buf), error);

	boost::asio::async_read(socket_, response_, boost::asio::transfer_at_least(1), boost::bind(&CommandTCPConnection::handle_read_content, this, boost::asio::placeholders::error));


	if (error == boost::asio::error::eof)
	{
		std::cout << "Reached EOF after calling async_read()....\n";
	}    
	else if (error)
	{
		std::cout << "Some error in async_read() before EOF was reached....\n";
		throw boost::system::system_error(error); // Some other error.
	}
	else
	{
		std::cout << "No error, continuing while(1) loop....\n";

	}

    message_ = "Command Accepted.\n";
    std::cout << "in the public method CommandTCPConnection::start(), about to call async_write()...\n";
    boost::asio::async_write(socket_, boost::asio::buffer(message_), boost::bind(&CommandTCPConnection::handle_write, shared_from_this())); 
	std::cout << "i am in the public method CommandTCPConnection::start(), LEAVING....\n";
}

CommandTCPConnection::CommandTCPConnection(boost::asio::io_service& io_service) : socket_(io_service)
{
	std::cout << "i am in the private constructor for CommandTCPConnection()\n";
}

void CommandTCPConnection::handle_write()
{
	std::cout << "i am in the private method for CommandTCPConnection::handle_write()\n";
}

