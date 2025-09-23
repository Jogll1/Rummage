#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "board.hpp"

#include "ui/ui.hpp"

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

		sf::RenderWindow* m_window;
		sf::VideoMode m_videoMode;
		sf::View m_view;

		// Game

		Board* m_board; // Where tiles will be played to
		Board* m_hand;  // Where tiles will be played from

		std::vector<std::unique_ptr<Tile>> m_deck;

		std::unique_ptr<Tile> m_currentTile = nullptr; // Only allow one tile to be moved at a time
		Slot* m_lastSlot = nullptr;                    // Default to a slot once a move is cancelled

		bool m_gameStarted = false;

		// UI

		std::unique_ptr<UI> m_mainMenuUI = nullptr;
		std::unique_ptr<UI> m_gameUI = nullptr;

		// Private functions

		void initVariables();
		void initWindow();

		sf::Vector2f getGameWorldSize() const;
		sf::Vector2f getGameWorldCentre() const;
		sf::Vector2f getMousePosView() const { return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)); }

		std::unique_ptr<UI> createMenuUI();
		std::unique_ptr<UI> createGameUI();

		void createDeck();
		void drawToHand();
		bool canSwap(Slot& from, Slot& to);

		void handleDragAndDrop(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event);
		void pollEvents();

		void openJoinMenu();
		void closeJoinMenu();
		void startGame();
		void closeWindow();

		void resizeView(int windowWidth, int windowHeight);
	public:
		Game();
		virtual ~Game();

		// Public functions
		
		void update();
		void render();
		
		// Getters

		bool isRunning() const;
	};
}