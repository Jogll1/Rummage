#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

namespace Rummage 
{
	// Wrapper class for the game engine

	class Game
	{
	private:
		static const int kWindowWidth  = 1200;
		static const int kWindowHeight = 900;

		static const int kWindowStyle = sf::Style::Titlebar | sf::Style::Close;

		sf::RenderWindow* window;
		sf::VideoMode video_mode;

		void initVariables();
		void initWindow();

	public:
		Game();
		virtual ~Game();

		// Public functions
		
		void pollEvents();
		void update();
		void render();
		
		// Accessors

		bool isRunning() const;
	};
}