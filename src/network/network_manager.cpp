#include "network_manager.hpp"

#include <iostream>

//#ifdef _WIN32
//#include <winsock2.h>
//#else
//#include <arpa/inet.h>
//#endif

#include <SFML/Network.hpp>
#include <nlohmann/json.hpp>

namespace Rummage
{
	inline bool NetworkManager::isValidMessage(const json& data, bool checkPayload)
	{
		// Validate JSON according to message protocol
		return data.contains("type") && data.contains("action") && (!checkPayload || data.contains("payload"));
	}

	bool NetworkManager::sendMessageUDP(sf::UdpSocket& socket, sf::IpAddress ip, unsigned short port, const json& data)
	{
		// Message must follow protocol defined in protocol.md
		if (!isValidMessage(data, false)) return false;

		// Serialise JSON
		std::string jsonString = data.dump();

		// Send
		if (socket.send(jsonString.c_str(), jsonString.size(), ip, port) != sf::Socket::Status::Done) return false;

		return true;
	}

	bool NetworkManager::sendMessageTCP(sf::TcpSocket& socket, const json& data)
	{
		// Message must follow protocol defined in protocol.md
		if (!isValidMessage(data, false)) return false;

		// Serialise JSON
		std::string jsonString = data.dump();

		// Send
		if (socket.send(jsonString.c_str(), jsonString.size()) != sf::Socket::Status::Done) return false;

		return true;
	}

	sf::IpAddress NetworkManager::findLocalHost()
	{
		sf::UdpSocket socket;
		socket.setBlocking(false);
		unsigned short port = sf::Socket::AnyPort;
		sf::Socket::Status status = socket.bind(port);

		if (status != sf::Socket::Status::Done)
		{
			// ERROR!
			std::cerr << "[ERROR] Failed to bind socket at " << port << "\n";
			return sf::IpAddress::LocalHost;
		}

		// Send JSON Message

		const json sendData = {
			{"type", "request"},
			{"action", "get_local_host"}
		};

		unsigned short targetPort = 54001;
		if (!sendMessageUDP(socket, sf::IpAddress::Broadcast, targetPort, sendData))
		{
			// ERROR!
			std::cerr << "[ERROR] Failed to broadcast discovery at " << targetPort << "\n";
			return sf::IpAddress::LocalHost;
		}

		std::cout << "[UDP] Waiting for broadcast response.\n";

		// Wait for response from server

		sf::Clock clock;
		const sf::Time timeout = sf::seconds(m_maxResponseWait);

		std::array<char, 1024> responseData = { 0 };
		std::size_t received;
		std::optional<sf::IpAddress> senderIp;
		unsigned short senderPort;

		while (clock.getElapsedTime() < timeout)
		{
			sf::Socket::Status status = socket.receive(responseData.data(), responseData.size(), received, senderIp, senderPort);

			if (status == sf::Socket::Status::Done)
			{
				try
				{
					std::string raw(responseData.data(), received);
					json parsed = json::parse(raw);

					// Validate 
					if (isValidMessage(parsed, true)) 
					{
						if (parsed["type"] == "response" && parsed["action"] == "get_local_host")
						{
							std::string_view ipStr = parsed["payload"]["host_ip"];
							auto resolve = sf::IpAddress::resolve(ipStr);

							if (resolve.has_value())
							{
								return resolve.value();
							}
							
							std::cerr << "[ERROR] Invalid host IP received.\n";
						}
					}
				}
				catch (const std::exception& e)
				{
					std::cerr << "[ERROR] Failed to parse discovery response: " << e.what() << "\n";
				}
			}

			// Sleep briefly to prevent CPU spinning
			sf::sleep(sf::milliseconds(10));
		}

		std::cerr << "[FAIL] Failed to discover a local server.";
		return sf::IpAddress::LocalHost;
	}

	bool NetworkManager::connectToServer(const sf::IpAddress ip, unsigned short port)
	{
		sf::Socket::Status status = m_socket.connect(ip, port);

		if (status != sf::Socket::Status::Done)
		{
			std::cerr << "[ERROR] Failed to connect to server at " << ip.toString() << ":" << port << "\n";
			return false;
		}

		std::cerr << "[CONNECTED] Connected to server at " << ip.toString() << ":" << port << "\n";
		return true;
	}

	void NetworkManager::startReceiving()
	{
		m_receiving = true;
		m_receiveThread = std::thread(&NetworkManager::receiveLoop, this);
	}

	void NetworkManager::stopReceiving()
	{
		m_receiving = false;
		if (m_receiveThread.joinable())
		{
			m_receiveThread.join();
		}
		m_socket.disconnect();
	}

	void NetworkManager::receiveLoop()
	{
		while (m_receiving)
		{
			std::array<char, 1024> responseData = { 0 };
			std::size_t received;
			sf::Socket::Status status = m_socket.receive(responseData.data(), responseData.size(), received);

			switch (status)
			{
			case sf::Socket::Status::Done: 
			{
				// Sucessfully received packet
				std::cout << "[SERVER] " << responseData.data() << "\n";
				receiveMessage(std::string(responseData.data(), received));
				break;
			}
			case sf::Socket::Status::Disconnected: 
			{
				std::cout << "[DISCONNECTED] Server disconnected.\n";
				m_receiving = false;
				break;
			}
			case sf::Socket::Status::Error:
			{
				std::cerr << "[ERROR] Socket error.\n";
				m_receiving = false;
				break;
			}
			default: sf::sleep(sf::milliseconds(10)); break;
			}
		}
	}

	void NetworkManager::receiveMessage(const std::string raw)
	{
		try
		{
			json message = json::parse(raw);

			// Handle messages received from the server
			if (!isValidMessage(message, true)) return;

			// Responses
			if (message["type"] == "response")
			{
				if (message["action"] == "create_room")
				{
					// Store room code
					m_currentRoom = message["payload"]["room_code"];
				}
				else if  (message["action"] == "join_room")
				{
					// Store room code
					bool connected = message["payload"]["connected"];

					if (connected) 
					{
						//m_currentRoom = 
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "[ERROR] Failed to parse discovery response: " << e.what() << "\n";
		}
	}

	// Constructor and Deconstructor

	NetworkManager::NetworkManager() {}

	NetworkManager::~NetworkManager()
	{
		stopReceiving();
	}

	// Public functions

	bool NetworkManager::hostGame()
	{
		// Connect to server
		m_hostAddress = findLocalHost();
		bool connected = connectToServer(m_hostAddress, 54000);

		if (!connected) return false;

		// Request new game to be created

		const json sendData = {
			{"type", "request"},
			{"action", "create_room"}
		};

		if (!sendMessageTCP(m_socket, sendData))
		{
			// ERROR!
			std::cerr << "[ERROR] Failed to send room creation request. \n";
			return false;
		}

		startReceiving();

		return connected;
	}

	bool NetworkManager::joinGame(std::string code)
	{
		// Connect to server
		m_hostAddress = findLocalHost();
		bool connected = connectToServer(m_hostAddress, 54000);

		if (!connected) return false;

		// Request to join game

		const json sendData = {
			{"type", "request"},
			{"action", "join_room"},
			{"payload", code }
		};

		if (!sendMessageTCP(m_socket, sendData))
		{
			// ERROR!
			std::cerr << "[ERROR] Failed to send room join request. \n";
			return false;
		}

		startReceiving();

		return connected;
	}
}