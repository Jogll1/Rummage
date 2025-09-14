#include "game.hpp"

#include <iostream>
#include <time.h>

#include "board.hpp"

namespace Rummage
{
	// Private functions

	void Game::initVariables()
	{
		m_window = nullptr;
		m_board = new Board(11u, 11u);
	}

	void Game::initWindow()
	{
		m_videoMode = sf::VideoMode({ kInitWindowWidth, kInitWindowHeight });
		m_window = new sf::RenderWindow(m_videoMode, "Rummage", kWindowStyle);

		m_view = sf::View(sf::FloatRect(sf::Vector2f(), m_board->getSize()));
		m_view.setCenter(m_board->getSize() / 2.f);
		m_window->setView(m_view);

		m_window->setVerticalSyncEnabled(true); // VSync
		//this->window.setFramerateLimit(60);

		resizeView(kInitWindowWidth, kInitWindowHeight);
	}

	void Game::resizeView(int windowWidth, int windowHeight)
	{
		float windowRatio = (float)windowWidth / (float)windowHeight;
		float viewRatio = m_view.getSize().x / m_view.getSize().y;

		float sizeX = 1.0f, sizeY = 1.0f;
		float posX = 0, posY = 0;

		if (windowRatio > viewRatio)
		{
			// Horizontal letterboxing
			sizeX = viewRatio / windowRatio;
			posX = (1.0f - sizeX) / 2.0f;
		}
		else
		{
			// Vertical letterboxing
			sizeY = windowRatio / viewRatio;
			posY = (1.0f - sizeY) / 2.0f;
		}

		std::cout << "PosX: " << posX << ", PosY: " << posY << "\n";
		std::cout << "SizeX: " << sizeX << ", SizeY: " << sizeY << "\n";

		m_view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
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
		m_window->clear(/*sf::Color(92, 214, 92)*/);

		// Draw background
		sf::RectangleShape background(sf::Vector2f(m_view.getSize().x, m_view.getSize().y));
		background.setFillColor(sf::Color(92, 214, 92));
		m_window->draw(background);

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