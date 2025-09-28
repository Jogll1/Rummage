#pragma once

#include <thread>
#include <atomic>
#include <mutex>

#include <SFML/Network.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Rummage
{
	class NetworkManager
	{
	private:
		// Network

		sf::IpAddress m_hostAddress = sf::IpAddress::LocalHost;
		float m_maxResponseWait = 5.f;

		sf::TcpSocket m_socket;
		std::thread m_receiveThread;
		std::atomic<bool> m_receiving{ false };

		std::mutex m_sendMutex;

		// Game

		std::string m_currentRoom = ""; // Current room code

		// Private functions

		inline bool isValidMessage(const json& data, bool checkPayload);
		bool sendMessageUDP(sf::UdpSocket& socket, sf::IpAddress ip, unsigned short port, const json& data);
		bool sendMessageTCP(sf::TcpSocket& socket, const json& data);

		sf::IpAddress findLocalHost();
		bool connectToServer(const sf::IpAddress ip, unsigned short port);

		void startReceiving();
		void stopReceiving();
		void receiveLoop();

		void receiveMessage(const std::string raw);
	public:
		NetworkManager();
		~NetworkManager();

		// Getters

		std::string getCurrentRoom() { return m_currentRoom; }

		// Public functions

		bool hostGame();
		bool joinGame(std::string code);
	};
}