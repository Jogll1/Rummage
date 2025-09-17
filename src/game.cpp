#include "game.hpp"

#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>

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

		// Create deck
		createDeck();
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

	void Game::createDeck()
	{
		// 2 * 4 suits * 12 ranks + 2 jokers = 98 tiles
		for (size_t i = 0; i < 2; i++)
		{
			// Suit
			for (int s = SUIT_GOLD; s < SUIT_MAX; s++)
			{
				// Rank
				for (int r = RANK_A; r < RANK_MAX; r++)
				{
					m_deck.push_back(Tile::createTile(static_cast<Suit>(s), static_cast<Rank>(r)));
				}
			}

			// Joker
			m_deck.push_back(Tile::createTile(SUIT_NONE, RANK_NONE));
		}

		// Shuffle the elements
		for (size_t i = m_deck.size() - 1; i > 0; i--)
		{
			size_t j = rand() % i;
			std::iter_swap(m_deck.begin() + i, m_deck.begin() + j);
		}
	}

	// Can place a tile if:
	// It has no immediate neighbours in the cardinal directions OR
	// It forms a sequence:
	// - of the same rank OR
	// - of the same suit where ranks go up by 1 in the same direction (i.e. A, 2, 3 not A, 2, A)
	// Jokers can be any suit or rank
	bool Game::canPlace(Slot* slot, sf::Vector2u slotCoords, const std::unique_ptr<Board>* board)
	{
		if (m_currentTile && *board)
		{
			// Always place in hand 
			if (*board == m_hand) return true;

			// Checks for same rank: GOLD, SILVER, COPPER, IRON
			int suitsMatched[SUIT_MAX - 1] = { 
				m_currentTile->getSuit() == SUIT_GOLD ? 1 : 0,
				m_currentTile->getSuit() == SUIT_SILVER ? 1 : 0,
				m_currentTile->getSuit() == SUIT_COPPER ? 1 : 0,
				m_currentTile->getSuit() == SUIT_IRON ? 1 : 0
			};

			// Check for same suit
			int diff = 0;

			// Check the cardinal directions

			// Left
			for (int x = slotCoords.x - 1; x >= 0; x--)
			{
				Slot* slot = (*board)->getSlotAt(x, slotCoords.y);
				
				// Empty tile
				if (!slot->hasTile()) break;

				Rank r = (*slot->getTile())->getRank();
				Suit s = (*slot->getTile())->getSuit();

				// Same rank
				if (r == m_currentTile->getRank())
				{
					std::cout << "same rank\n";
					++suitsMatched[s - 1];
					std::cout << suitsMatched[0] << ", " << suitsMatched[1] << ", " << suitsMatched[2] << ", " << suitsMatched[3] << "\n";
					// Increment no. instances of s and check if its more than 1
					if (suitsMatched[s - 1] > 1)
					{
						return false;
					}
				}

				return false;
			}

			return true;
		}

		return false;
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
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				// If right click, cancel move
				if (mouseButtonPressed->button == sf::Mouse::Button::Right)
				{
					m_lastSlot->setTile(std::move(m_currentTile));
				}
			}
			else
			{
				// If left release, drop tile accordingly
				const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>();

				// For the slots in m_board and m_hand, place tile if mouse is released over a slot
				// Continuously loop over slots to update the slot outline
				for (const auto* board : { &m_board, &m_hand })
				{
					if (*board)
					{
						for (unsigned int y = 0; y < (*board)->getSlotsY(); y++)
						{
							for (unsigned int x = 0; x < (*board)->getSlotsX(); x++)
							{
								Slot* slot = (*board)->getSlotAt(x, y);

								if (slot->isMouseOver(mousePosView) && canPlace(slot, { x, y }, board))
								{
									// Slot is playable
									slot->setOutline(true);

									if (mouseButtonReleased)
									{
										m_currentTile->setIsMoving(false);

										if (slot->hasTile())
										{
											// Swap tiles at that slot
											std::unique_ptr<Tile> temp = slot->dropTile();
											slot->setTile(std::move(m_currentTile));
											m_lastSlot->setTile(std::move(temp));
										}
										else
										{
											// Drop current tile into new slot
											slot->setTile(std::move(m_currentTile));
										}

										m_lastSlot = slot;
										slot->setOutline(false);
										return;
									}
								}
								else
								{
									// Slot is not playable
									slot->setOutline(false);
								}
							}
						}
					}
				}

				// Return current tile to last slot if not dropped
				if (mouseButtonReleased && m_lastSlot)
				{
					m_lastSlot->setTile(std::move(m_currentTile));
				}
			}
		}
		else
		{
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
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

				if (mouseButtonPressed->button == sf::Mouse::Button::Right)
				{
					// Send back to hand if tile is on the board
					if (!m_hand->isFull())
					{
						for (auto& slot : *m_board->getSlots())
						{
							if (slot.isMouseOver(mousePosView) && slot.hasTile())
							{
								m_hand->sendTileToFirstSlot(std::move(slot.dropTile()));
							}
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
					{
						m_window->close();
						break;
					}
					case sf::Keyboard::Scancode::D:
					{
						if (m_hand)
						{
							m_hand->drawTileFromDeck(m_deck);
						}
						break;
					}
					default: break;
				}
			}

			handleDragAndDrop(event);
		}
	}

	void Game::resizeView(int windowWidth, int windowHeight)
	{
		// Make view fit the game board while keeping the board's aspect and an integer scaling

		// Calculate view size based on integer scale
		int scale = std::max(1.f, std::min(windowWidth / getGameWorldSize().x, windowHeight / getGameWorldSize().y));

		float viewWidth = std::floor(static_cast<float>(windowWidth) / scale);
		float viewHeight = std::floor(static_cast<float>(windowHeight) / scale);

		m_view.setSize({ viewWidth, viewHeight });
		m_view.setCenter({ std::floor(getGameWorldCentre().x), std::floor(getGameWorldCentre().y) });

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

		//m_board->update(mousePosView, m_currentTile != nullptr);
		//m_hand->update(mousePosView, m_currentTile != nullptr);

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