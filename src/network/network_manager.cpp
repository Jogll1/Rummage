#include "network_manager.hpp"

#include <iostream>

#include <SFML/Network.hpp>

namespace Rummage
{
	bool NetworkManager::connectToServer(const sf::IpAddress ip, unsigned short port)
	{
		sf::Socket::Status status = socket.connect(ip, port);

		if (status != sf::Socket::Status::Done)
		{
			std::cerr << "Failed to connect to server at " << ip.toString() << ":" << port << "\n";
			return false;
		}

		socket.setBlocking(true);
		std::cerr << "Connected to server at " << ip.toString() << ":" << port << "\n";
		return true;
	}

	// Constructor

	NetworkManager::NetworkManager()
	{
		if (auto local = sf::IpAddress::getLocalAddress())
		{
			std::cout << "Local IP = " << local->toString() << "\n";
			connectToServer(*local, 5050);
		}
		else
		{
			std::cout << "Can't find local IP";
		}
	}
}