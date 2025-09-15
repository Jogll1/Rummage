#include "game.hpp"

#include <iostream>
#include <time.h>

#include "board.hpp"
#include "utils.hpp"

namespace Rummage
{
	// Private functions

	void Game::initVariables()
	{
		m_window = nullptr;

		// Create a new centered board
		m_board = new Board(11u, 11u);
		m_board->setPos({ kInitWindowWidth / 2 - m_board->getSize().x / 2, kInitWindowHeight / 2 - m_board->getSize().y / 2 });
	}

	void Game::initWindow()
	{
		m_videoMode = sf::VideoMode({ kInitWindowWidth, kInitWindowHeight });
		m_window = new sf::RenderWindow(m_videoMode, "Rummage", kWindowStyle);

		// Set the view to centre on the board
		resizeView(kInitWindowWidth, kInitWindowHeight);

		// Set FPS
		m_window->setVerticalSyncEnabled(true); // VSync
		//this->window.setFramerateLimit(60);
	}

	void Game::resizeView(int windowWidth, int windowHeight)
	{
		// Make view fit the game board while keeping the board's aspect

		float windowRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
		float boardRatio = m_board->getSize().x / m_board->getSize().y;

		float width = m_board->getSize().x;
		float height = m_board->getSize().y;

		if (windowRatio >= boardRatio)
		{
			// Height match
			width = height * windowRatio;
			width = std::floor(width);
		}	
		else
		{
			// Width match
			height = width / windowRatio;
			height = std::floor(height);
		}

		m_view.setSize({ width, height });

		sf::Vector2f center = m_board->getCentrePos();
		center.x = std::floor(center.x) + 0.5f;
		center.y = std::floor(center.y) + 0.5f;
		m_view.setCenter(center);

		m_window->setView(m_view);
	}

	// Constructor and Destructor

	Game::Game()
	{ 
		srand(time(NULL));

		initVariables();
		initWindow();
	}

	Game::~Game()
	{
		delete m_window;
		delete m_board;
	}

	// Public functions

	void Game::pollEvents()
	{
		while (const std::optional event = m_window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				m_window->close();
			}
			
			if (const auto* resized = event->getIf<sf::Event::Resized>())
			{
				resizeView(resized->size.x, resized->size.y);
			}

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				switch (keyPressed->scancode)
				{
					case sf::Keyboard::Scancode::Escape:
						m_window->close();
						break;
					default: break;
				}
			}

			m_board->handleEvents(*m_window, event);
		}
	}

	void Game::update()
	{
		pollEvents();

		sf::Vector2f mousePosView = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
		m_board->update(mousePosView);
	}

	void Game::render()
	{
		m_window->clear(sf::Color(92, 214, 92));

		// Draw background
		/*sf::RectangleShape background(sf::Vector2f(m_view.getSize().x, m_view.getSize().y));
		background.setFillColor(sf::Color(92, 214, 92));
		m_window->draw(background);*/

		// Draw board
		if (m_board)
		{
			m_window->draw(*m_board);
		}

		m_window->display();
	}

	// Getters

	bool Game::isRunning() const
	{
		return m_window->isOpen();
	}
}