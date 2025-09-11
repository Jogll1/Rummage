#include "game.hpp"

namespace Rummage
{
	// Private functions

	void Game::initVariables()
	{
		this->window = nullptr;
	}

	void Game::initWindow()
	{
		this->video_mode = sf::VideoMode({ kWindowWidth, kWindowHeight });
		this->window = new sf::RenderWindow(this->video_mode, "Rummage", kWindowStyle);

		this->window->setVerticalSyncEnabled(true); // VSync
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
		delete this->window;
	}

	// Public functions

	void Game::pollEvents()
	{
		while (const std::optional event = this->window->pollEvent())
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
		this->pollEvents();
	}

	void Game::render()
	{
		this->window->clear();

		//this->window->draw(bg_sprite);

		this->window->display();
	}

	// Accessors

	bool Game::isRunning() const
	{
		return this->window->isOpen();
	}
}