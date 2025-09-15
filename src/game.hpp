#pragma once

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "board.hpp"

namespace Rummage 
{
	// Wrapper class for the game engine

	class Game
	{
	private:
		static const unsigned int kInitWindowWidth  = 1200u;
		static const unsigned int kInitWindowHeight = 900u;

		static const int kWindowStyle = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;

		sf::RenderWindow* m_window;
		sf::VideoMode m_videoMode;
		sf::View m_view;

		// Game objects

		std::unique_ptr<Board> m_board; // Where tiles will be played to
		std::unique_ptr<Board> m_hand;  // Where tiles will be played from

		std::unique_ptr<Tile> m_currentTile = nullptr; // Only allow one tile to be moved at a time
		Slot* m_lastSlot = nullptr;                     // Default to a slot once a move is cancelled

		// Private functions

		void initVariables();
		void initWindow();

		sf::Vector2f getGameWorldSize() const { return WorldObject::getBoundingBoxSize({ *m_board, *m_hand }); }
		sf::Vector2f getGameWorldCentre() const { return WorldObject::getBoundingBoxCentre({ *m_board, *m_hand }); }
		sf::Vector2f getMousePosView() const { return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)); }

		bool pickupTileFromSlot(Slot& slot, sf::Vector2f mousePosView);

		void handleDragAndDrop(const std::optional<sf::Event> event);
		void pollEvents();

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