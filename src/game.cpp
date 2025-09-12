#include "game.hpp"

#include "board.hpp"

namespace Rummage
{
	// Private functions

	void Game::initVariables()
	{
		window = nullptr;
		board = new Board(11u, 11u);
	}

	void Game::initWindow()
	{
		videoMode = sf::VideoMode({ kWindowWidth, kWindowHeight });
		window = new sf::RenderWindow(this->videoMode, "Rummage", kWindowStyle);

		view = sf::View(sf::FloatRect(sf::Vector2f(), board->getSize()));
		view.setCenter(board->getSize() / 2.f);
		window->setView(view);

		window->setVerticalSyncEnabled(true); // VSync
		//this->window.setFramerateLimit(60);
	}

	// Constructor and Destructor

	Game::Game()
	{ 
		initVariables();
		initWindow();
	}

	Game::~Game()
	{
		delete window;

		delete board;
	}

	// Public functions

	void Game::pollEvents()
	{
		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window->close();
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					window->close();
				}
			}
		}
	}

	void Game::update()
	{
		pollEvents();
	}

	void Game::render()
	{
		window->clear(sf::Color(92, 214, 92));

		if (board != nullptr)
		{
			board->draw(*window);
		}

		window->display();
	}

	// Getters

	bool Game::isRunning() const
	{
		return window->isOpen();
	}
}