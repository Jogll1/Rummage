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
		m_board = new Board(11u, 11u);

		Padding pad = { 10, 10, 10, 10 };
		m_hand = new Board(11u, 2u, pad);

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
			for (int s = 1; s < SUIT_MAX; s++)
			{
				// Rank
				for (int r = 1; r < RANK_MAX; r++)
				{
					m_deck.push_back(Tile::createTile(static_cast<Suit>(s), static_cast<Rank>(r)));
				}
			}

			// Joker
			//m_deck.push_back(Tile::createTile(SUIT_NONE, RANK_NONE));
		}

		// Shuffle the elements
		for (size_t i = m_deck.size() - 1; i > 0; i--)
		{
			size_t j = rand() % i;
			std::iter_swap(m_deck.begin() + i, m_deck.begin() + j);
		}
	}

	// Swaps the tiles at From and To if they are valid in their destinations.
	// A tile is valid if:
	// - It has no immediate neighbours in the cardinal directions OR
	// - It forms a sequence:
	//   - of the same rank OR
	//   - of the same suit where ranks go up by 1 in the same direction (i.e. A, 2, 3 not A, 2, A)
	// Jokers can be any suit or rank.
	bool Game::canSwap(Slot& from, Slot& to)
	{
		// Assumes that m_currentTile is the tile represented by from
		if (!m_currentTile) return false;

		// Always allow hand to hand or same slot to same slot swapping
		if (from.getParent() == m_hand && to.getParent() == m_hand || &from == &to) return true;

		// Rule checking lambda - always goes left to right or top to bottom
		// Returns whether a given array doesn't violates the rules.
		auto checkRules = [] (std::vector<sf::Vector2i> rep) {
			// Check for same rank
			int suitsMatched[SUIT_MAX - 1] = { 0, 0, 0, 0 };

			// Checks for same suit
			int diff = 0;

			// Which check we are doing: -1 for none, 0 for same rank, 1 for same suit
			int lastCheck = -1;

			sf::Vector2i lastTile = { -1, -1 };

			for (size_t i = 0; i < rep.size(); i++)
			{
				if (rep[i].x == -1)
				{
					// Empty tile - reset checks
					std::fill(suitsMatched, suitsMatched + SUIT_MAX - 1, 0);
					diff = 0;
					lastCheck = -1;
				}
				else if (lastTile.x != -1)
				{
					// Non-empty tile - compare to lastTile
					int check = -1;

					if (rep[i].y == lastTile.y)
					{
						// Same rank as lastTile
						if (lastCheck != 0)
						{
							++suitsMatched[lastTile.x - 1];
						}

						check = 0;

						if (++suitsMatched[rep[i].x - 1] > 1)
						{
							return false;
						}
					}
					else if (rep[i].x == lastTile.x)
					{
						// Same suit as lastTile
						check = 1;

						int checkDiff = lastTile.y - rep[i].y;
						if (std::abs(checkDiff) != 1) return false;

						if (diff == 0)
						{
							diff = checkDiff;
						}
						else if (checkDiff != diff)
						{
							return false;
						}
					}
					else
					{
						return false;
					}

					if (lastCheck == -1)
					{
						lastCheck = check;
					}

					if (check != lastCheck) return false;
				}

				lastTile = rep[i];
			}

			return true;
		};

		// Create array representations of the rows and columns being changed
		// Where each member is (Suit, Rank) and empties are (-1, -1)
		std::vector<sf::Vector2i> toRow(to.getParent()->getSlotsX());
		std::vector<sf::Vector2i> toCol(to.getParent()->getSlotsY());

		std::vector<sf::Vector2i> fromRow(from.getParent()->getSlotsX());
		std::vector<sf::Vector2i> fromCol(from.getParent()->getSlotsY());

		bool sameRow = to.getCoords().y == from.getCoords().y;
		bool sameCol = to.getCoords().x == from.getCoords().x;

		// Initialise row/col if:
		// The board of the row/col is not m_hand

		// To: This is where m_currentTile is going, and where the tile at From is going.
		// When initialising To, m_currentTile should be added.
		//
		// From, i.e. m_lastSlot: This is where m_currentTile is coming from, and the tile
		// at To is going.
		// When initialising From, the tile at To should be added if To has a tile.
		// If To doesn't have a tile, don't check From.
		//
		// If both From and To have the same row or column, it shouldn't be checked twice.
		// Order: To row, To col, From row, From col,
		//		  given From row != To row, and From col != To col

		auto initVectorRep = [sameRow, sameCol, &from, &to] (
			std::vector<sf::Vector2i>& vector, std::unique_ptr<Tile>* currentTile, bool isRow, bool isTo
		) {
			Slot& current = isTo ? to : from;

			for (size_t i = 0; i < vector.size(); i++)
			{
				unsigned int x = isRow ? i : current.getCoords().x;
				unsigned int y = isRow ? current.getCoords().y : i;

				if (Slot* slot = current.getParent()->getSlotAt(x, y))
				{
					if (slot->hasTile())
					{
						vector[i] = { slot->getTile()->getSuit(), slot->getTile()->getRank() };
					}
					else
					{
						vector[i] = { -1, -1 };
					}
				}
			}

			if (isTo)
			{
				// Add in m_currentTile
				unsigned int index = isRow ? to.getCoords().x : to.getCoords().y;
				vector[index] = { (*currentTile)->getSuit(), (*currentTile)->getRank()};
			}
			
			if (to.hasTile() && ((!isTo) || (isRow && sameRow) || (!isRow && sameCol)))
			{
				unsigned int index = isRow ? from.getCoords().x : from.getCoords().y;
				vector[index] = { to.getTile()->getSuit(), to.getTile()->getRank() };
			}
		};

		// ============ To ============
		if (to.getParent() != m_hand)
		{
			// ============ To row ============
			initVectorRep(toRow, &m_currentTile, true, true);
			if (!checkRules(toRow)) return false;

			// ============ To col ============
			initVectorRep(toCol, &m_currentTile, false, true);
			if (!checkRules(toCol)) return false;
		}
		
		// ============ From ============
		if (to.hasTile() && from.getParent() != m_hand)
		{
			if (!sameRow)
			{
				// ============ From row ============
				initVectorRep(fromRow, &m_currentTile, true, false);
				if (!checkRules(fromRow)) return false;
			}

			if (!sameCol)
			{
				// ============ From col ============
				initVectorRep(fromCol, &m_currentTile, false, false);
				if (!checkRules(fromCol)) return false;
			}
		}

		return true;
	}

	// Handle dragging an dropping tiles in slots.
	// Tiles can be dragged between the hand and the board.
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
					// Deselect all slots from being highlighted
					for (auto* board : { m_board, m_hand })
					{
						for (Slot& slot : *board->getSlots())
						{
							slot.setOutline(false);
						}
					}

					m_lastSlot->setTile(std::move(m_currentTile));
				}
			}
			else
			{
				// If left released, drop tile accordingly
				const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>();

				// For the slots in m_board and m_hand, place tile if mouse is released over a slot
				// Continuously loop over slots to update the slot outline
				for (auto* board : { m_board, m_hand })
				{
					if (board)
					{
						for (Slot& slot : *board->getSlots())
						{
							if (slot.isMouseOver(mousePosView))
							{
								if (canSwap(*m_lastSlot, slot))
								{
									// Slot is playable
									slot.setOutline(true);

									if (mouseButtonReleased)
									{
										m_currentTile->setIsMoving(false);

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
										slot.setOutline(false);
										return;
									}
								}
							}
							else
							{
								// Slot is not playable
								slot.setOutline(false);
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

		delete m_board;
		delete m_hand;
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