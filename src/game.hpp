#pragma once

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

		// Objects

		Board* m_board;

		// Private functions

		void initVariables();
		void initWindow();

		void resizeView(int windowWidth, int windowHeight);
	public:
		Game();
		virtual ~Game();

		// Public functions
		
		void pollEvents();
		void update();
		void render();
		
		// Getters

		bool isRunning() const;
	};
}