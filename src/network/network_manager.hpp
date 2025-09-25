#pragma once

#include <thread>
#include <functional>
#include <mutex>

#include <SFML/Network.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Rummage
{
	class NetworkManager
	{
	private:
		bool connectToServer(const sf::IpAddress ip, unsigned short port);
	public:
		NetworkManager();

		sf::TcpSocket socket;
		std::thread recieveThread;
		std::function<void(const json&)> onMessageReceived;

		std::mutex sendMutex;
	};
}