#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "board.hpp"
#include "scenes/scene_manager.hpp"
#include "network/network_manager.hpp"

namespace Rummage 
{
	// Wrapper class for the game engine

	class Game
	{
	private:
		// Window

		static const unsigned int kInitWindowWidth  = 1200u;
		static const unsigned int kInitWindowHeight = 900u;

		static const int kWindowStyle = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;

		std::unique_ptr<sf::RenderWindow> m_window;
		sf::VideoMode m_videoMode;
		sf::View m_view;

		// Scenes

		SceneManager m_sceneManager;

		// Network

		NetworkManager m_networkManager;

		// Game

		bool m_gameStarted = false;
		bool m_isMyTurn = false;

		// Private functions

		void initWindow();

		sf::Vector2f getGameWorldSize();
		sf::Vector2f getGameWorldCentre();
		sf::Vector2f getMousePosView() const { return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)); }

		void pollEvents(const sf::Vector2f& mousePosView);
		void resizeView(int windowWidth, int windowHeight);
		void checkScenes();
	public:
		Game();
		~Game() = default;

		// Getters

		bool isRunning() const;
		NetworkManager* getNetworkManager() { return &m_networkManager; }

		// Public functions
		
		void closeWindow();

		void update();
		void render();

		// Network functions

		void hostGame();
		void joinGame(const std::string code);
		std::string getGameCode();

		// Game

		void startGame();
		std::unique_ptr<Tile> requestTile();
	};
}