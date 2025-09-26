#include "network_manager.hpp"

#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

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
		// Must follow protocol defined in protocol.md

		// Serialise JSON
		std::string jsonString = data.dump();

		// Send
		if (socket.send(jsonString.c_str(), jsonString.size(), ip, port) != sf::Socket::Status::Done) return false;

		return true;
	}

	bool NetworkManager::sendMessageTCP(sf::TcpSocket& socket, sf::IpAddress ip, unsigned short port, const json& data)
	{
		// Must follow protocol defined in protocol.md

		// Serialise JSON
		std::string jsonString = data.dump();
		uint32_t length = htonl(jsonString.size());

		return false;
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

		// Get responses
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
		sf::Socket::Status status = socket.connect(ip, port);

		if (status != sf::Socket::Status::Done)
		{
			std::cerr << "[ERROR] Failed to connect to server at " << ip.toString() << ":" << port << "\n";
			return false;
		}

		std::cerr << "[CONNECTED] Connected to server at " << ip.toString() << ":" << port << "\n";
		return true;
	}

	// Constructor

	NetworkManager::NetworkManager()
	{
		sf::IpAddress serverIp = findLocalHost();
		connectToServer(serverIp, 54000);
	}
}