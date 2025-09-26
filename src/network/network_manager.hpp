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
		inline bool isValidMessage(const json& data, bool checkPayload);

		bool sendMessageUDP(sf::UdpSocket& socket, sf::IpAddress ip, unsigned short port, const json& data);
		bool sendMessageTCP(sf::TcpSocket& socket, sf::IpAddress ip, unsigned short port, const json& data);
		sf::IpAddress findLocalHost();
		bool connectToServer(const sf::IpAddress ip, unsigned short port);

		float m_maxResponseWait = 5.f;
	public:
		NetworkManager();

		sf::TcpSocket socket;
		std::thread recieveThread;
		std::function<void(const json&)> onMessageReceived;

		std::mutex sendMutex;
	};
}