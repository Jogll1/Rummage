#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <map>

#include <SFML/Network.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// TODO: 
// - Length prefix messages
// hostGame() and discovery code in a separate thread

namespace Rummage
{
	class Game;

	class NetworkManager
	{
	private:
		// Network

		sf::IpAddress m_hostAddress = sf::IpAddress::LocalHost;
		float m_maxResponseWait = 5.f;

		sf::TcpSocket m_socket;
		std::thread m_receiveThread;
		std::atomic<bool> m_receiving{ false };
		std::mutex m_responseMutex;
		std::map<std::string, std::promise<json>> m_pendingResponses;

		bool m_isConnected = false;

		// Game

		Game* m_game;
		std::string m_currentRoom = ""; // Current room code
		bool m_isPlayer1 = false;

		// Private functions

		inline bool isValidMessage(const json& data, bool checkPayload);
		sf::Socket::Status receiveBytesTCP(sf::TcpSocket& socket, void* buffer, std::size_t n);
		bool sendMessageTCP(sf::TcpSocket& socket, const json& data);
		bool sendMessageUDP(sf::UdpSocket& socket, sf::IpAddress ip, unsigned short port, const json& data);

		sf::IpAddress findLocalHost();
		bool connectToServer(const sf::IpAddress ip, unsigned short port);

		void startReceiving();
		void stopReceiving();
		void receiveLoop();

		void receiveMessage(const std::string raw);
	public:
		NetworkManager(Game* game);
		~NetworkManager();

		// Getters

		std::string getCurrentRoom() { return m_currentRoom; }
		const bool isPlayer1() const { return m_isPlayer1; }

		// Public functions

		bool hostGame();
		bool joinGame(const std::string code);
		bool requestGameStart();
	};
}