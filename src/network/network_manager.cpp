#include "network_manager.hpp"

#include <iostream>
#include <cstdint>
#include <vector>

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

	sf::Socket::Status NetworkManager::receiveBytesTCP(sf::TcpSocket& socket, void* buffer, std::size_t n)
	{
		// Helper to receive exactly n bytes from the socket
		sf::Socket::Status lastStatus = sf::Socket::Status::Done;

		std::size_t totalReceived = 0;
		char* dataPtr = static_cast<char*>(buffer);

		while (totalReceived < n)
		{
			std::size_t received = 0;
			sf::Socket::Status status = socket.receive(dataPtr + totalReceived, n - totalReceived, received);

			if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error)
			{
				std::cerr << "[ERROR] Connection lost during receive.\n";
				return status;
			}

			lastStatus = status;
			totalReceived += received;
		}

		return lastStatus;
	}

	bool NetworkManager::sendMessageTCP(sf::TcpSocket& socket, const json& data)
	{
		// Message must follow protocol defined in protocol.md
		if (!isValidMessage(data, false)) return false;

		// Serialise JSON
		std::string jsonString = data.dump();

		// Get length prefix
		uint32_t len = jsonString.size();
		uint8_t header[4] = { (len >> 24) & 0xFF, (len >> 16) & 0xFF, (len >> 8) & 0xFF, len & 0xFF };

		// Send
		std::vector<char> buffer(4 + len);
		std::memcpy(buffer.data(), header, 4);
		std::memcpy(buffer.data() + 4, jsonString.data(), len);

		if (socket.send(buffer.data(), buffer.size()) != sf::Socket::Status::Done) return false;

		return true;
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

		std::cerr << "[ERROR] Failed to discover a local server.\n";
		return sf::IpAddress::LocalHost;
	}

	bool NetworkManager::connectToServer(const sf::IpAddress ip, unsigned short port)
	{
		if (!m_isConnected)
		{
			sf::Socket::Status status = m_socket.connect(ip, port);

			if (status != sf::Socket::Status::Done)
			{
				std::cerr << "[ERROR] Failed to connect to server at " << ip.toString() << ":" << port << "\n";
				return false;
			}

			std::cerr << "[CONNECTED] Connected to server at " << ip.toString() << ":" << port << "\n";
		}

		m_isConnected = true;
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
			// Receive length prefix and convert to a uni32_t
			uint8_t buffer[4];
			if (receiveBytesTCP(m_socket, buffer, 4) != sf::Socket::Status::Done) break;

			uint32_t length = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
			if (length == 0) break;

			// Receive message
			std::vector<char> messageBuffer(length);
			sf::Socket::Status status = receiveBytesTCP(m_socket, messageBuffer.data(), length);

			std::string response(messageBuffer.begin(), messageBuffer.end());

			switch (status)
			{
			case sf::Socket::Status::Done:
			{
				// Sucessfully received packet
				std::string response(messageBuffer.begin(), messageBuffer.end());
				std::cout << "[SERVER] " << response << "\n";
				receiveMessage(response);
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

			std::string action = message["action"];

			// Fulfill the promise
			std::lock_guard<std::mutex> lock(m_responseMutex);
			auto it = m_pendingResponses.find(action);
			
			if (it != m_pendingResponses.end())
			{
				it->second.set_value(message);
				m_pendingResponses.erase(it);
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
		std::cout << "[ROOM] Attempting to host room.\n";

		// Connect to server
		if (!m_isConnected)
		{
			m_hostAddress = findLocalHost();
			bool connected = connectToServer(m_hostAddress, 54000);

			if (!connected) return false;
		}

		// Create promise/future
		std::promise<json> responsePromise;
		std::future<json> responseFuture = responsePromise.get_future();

		{
			std::lock_guard<std::mutex> lock(m_responseMutex);
			m_pendingResponses["create_room"] = std::move(responsePromise);
		}

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

		if (!m_receiving) startReceiving();

		// Wait for response from server (with timeout)
		if (responseFuture.wait_for(std::chrono::duration<float>(m_maxResponseWait)) == std::future_status::ready)
		{
			json response = responseFuture.get();

			// Check if the server created the room
			if (response["type"] == "response" && response["action"] == "create_room" &&
				response["payload"]["status"] == "success")
			{
				m_currentRoom = response["payload"]["room_code"];
				std::cout << "[ROOM] Room created successfully.\n";
				
				return true;
			}
		}
		
		std::cerr << "[ERROR] Failed to create room.\n";
		return false;
	}

	bool NetworkManager::joinGame(std::string code)
	{
		std::cout << "[ROOM] Attempting to join room at " << code << ".\n";

		// Connect to server
		if (!m_isConnected)
		{
			m_hostAddress = findLocalHost();
			bool connected = connectToServer(m_hostAddress, 54000);

			if (!connected) return false;
		}

		// Create promise/future
		std::promise<json> responsePromise;
		std::future<json> responseFuture = responsePromise.get_future();

		{
			std::lock_guard<std::mutex> lock(m_responseMutex);
			m_pendingResponses["join_room"] = std::move(responsePromise);
		}

		// Request to join game

		const json sendData = {
			{"type", "request"},
			{"action", "join_room"},
			{"payload", {{"room_code", code}} }
		};

		if (!sendMessageTCP(m_socket, sendData))
		{
			// ERROR!
			std::cerr << "[ERROR] Failed to send room join request. \n";
			return false;
		}

		if (!m_receiving) startReceiving();

		// Wait for response from server (with timeout)
		if (responseFuture.wait_for(std::chrono::duration<float>(m_maxResponseWait)) == std::future_status::ready)
		{
			json response = responseFuture.get();

			// Check if the server created the room
			if (response["type"] == "response" && response["action"] == "join_room" &&
				response["payload"]["status"] == "success")
			{
				std::cout << "[ROOM] Joined room " << code << ".\n";

				return true;
			}
		}

		std::cerr << "[ERROR] Failed to join room.\n";
		return false;
	}
}