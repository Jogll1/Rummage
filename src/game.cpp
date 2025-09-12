#include "game.hpp"

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
		m_videoMode = sf::VideoMode({ kWindowWidth, kWindowHeight });
		m_window = new sf::RenderWindow(m_videoMode, "Rummage", kWindowStyle);

		m_view = sf::View(sf::FloatRect(sf::Vector2f(), m_board->getSize()));
		m_view.setCenter(m_board->getSize() / 2.f);
		m_window->setView(m_view);

		m_window->setVerticalSyncEnabled(true); // VSync
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
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					m_window->close();
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
		m_window->clear(sf::Color(92, 214, 92));

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