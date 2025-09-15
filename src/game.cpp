#include "game.hpp"

#include <iostream>
#include <time.h>
#include <vector>

#include "board.hpp"
#include "utils.hpp"
#include "resources.hpp"

namespace Rummage
{
	// Private functions

	void Game::initVariables()
	{
		m_window = nullptr;

		// Create and position new board and hand
		m_board = std::make_unique<Board>(11u, 11u);

		Padding pad = { 10, 10, 10, 10 };
		m_hand = std::make_unique<Board>(11u, 2u, pad);

		//m_board->setPos({ kInitWindowWidth / 2 - m_board->getSize().x / 2, kInitWindowHeight / 2 - m_board->getSize().y / 2 });
		m_hand->setPos({ 0, m_board->getSize().y });
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

	// Handle dragging an dropping tiles in slots
	// Tiles can be dragged from: 
	// - the hand to the board, 
	// - the board to the board, or
	// - the hand to the hand, or
	void Game::handleDragAndDrop(const std::optional<sf::Event> event)
	{
		if (!event.has_value()) return;

		sf::Vector2f mousePosView = getMousePosView();

		if (m_currentTile)
		{
			if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
			{
				m_currentTile->setIsMoving(false);
				bool placed = false;

				// For the slots in m_board and m_hand, place tile
				// if mouse is release over a slot
				for (auto& slots : { m_board->getSlots(), m_hand->getSlots() })
				{
					for (Slot& slot : *slots)
					{
						if (slot.isMouseOver(mousePosView))
						{
							if (slot.hasTile())
							{
								// Swap tiles at that slot
								std::unique_ptr<Tile> temp = slot.dropTile();
								slot.setTile(std::move(m_currentTile));
								m_lastSlot->setTile(std::move(temp));
							}
							else
							{
								// Drop current tile into new slot
								slot.setTile(std::move(m_currentTile));
							}

							m_lastSlot = &slot;
							placed = true;
							break;
						}
					}
				}

				if (!placed && m_lastSlot)
				{
					// Return current tile to last slot if not dropped
					m_lastSlot->setTile(std::move(m_currentTile));
				}
			}
		}
		else
		{
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				// For the slots in m_board and m_hand, pickup tile
				// if mouse is pressed and slot has a tile
				for (auto& slots : { m_board->getSlots(), m_hand->getSlots() })
				{
					for (Slot& slot : *slots)
					{
						if (slot.isMouseOver(mousePosView) && slot.hasTile())
						{
							// Pickup tile
							m_lastSlot = &slot;
							m_currentTile = slot.dropTile();
							m_currentTile->setIsMoving(true);

							break;
						}
					}
				}
			}
		}
	}

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

			handleDragAndDrop(event);
		}
	}

	void Game::resizeView(int windowWidth, int windowHeight)
	{
		// Make view fit the game board while keeping the board's aspect

		sf::Vector2f gameWorldSize = getGameWorldSize();

		float windowRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
		float worldRatio = gameWorldSize.x / gameWorldSize.y;

		float width = gameWorldSize.x;
		float height = gameWorldSize.y;

		if (windowRatio >= worldRatio)
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

		sf::Vector2f center = getGameWorldCentre();
		center.x = std::floor(center.x) + 0.5f;
		center.y = std::floor(center.y) + 0.5f;
		m_view.setCenter(center);

		m_window->setView(m_view);
	}

	// Constructor and Destructor

	Game::Game()
	{ 
		srand(time(NULL));

		// Preload resources
		if (sf::Shader::isAvailable())
		{
			ResourceManager::preLoadShaders();
		}

		ResourceManager::preLoadTextures();

		initVariables();
		initWindow();
	}

	Game::~Game()
	{
		delete m_window;
	}

	// Public functions

	void Game::update()
	{
		pollEvents();

		sf::Vector2f mousePosView = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));

		m_board->update(mousePosView, m_currentTile != nullptr);
		m_hand->update(mousePosView, m_currentTile != nullptr);

		// Update dragged tile position
		if (m_currentTile)
		{
			m_currentTile->update(mousePosView);
		}
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

		// Draw hand
		if (m_hand)
		{
			m_window->draw(*m_hand);
		}

		// Draw dragged tile
		if (m_currentTile)
		{
			m_window->draw(*m_currentTile);
		}

		m_window->display();
	}

	// Getters

	bool Game::isRunning() const
	{
		return m_window->isOpen();
	}
}