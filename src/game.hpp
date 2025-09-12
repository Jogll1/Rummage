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
		static const int kWindowWidth  = 900;
		static const int kWindowHeight = 900;

		static const int kWindowStyle = sf::Style::Titlebar | sf::Style::Close;

		sf::RenderWindow* window;
		sf::VideoMode videoMode;
		sf::View view;

		// Objects

		Board* board;

		// Private functions

		void initVariables();
		void initWindow();
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