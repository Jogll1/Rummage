#include "game_scene.hpp"

#include <memory>

#include "../game.hpp"
#include "../resources.hpp"

#include "../ui/ui.hpp"
#include "../ui/button.hpp"
#include "../ui/input_field.hpp"
#include "../ui/image.hpp"
#include "../ui/group.hpp"
#include "../ui/text.hpp"

#include "../utils.hpp"

namespace Rummage
{
	// Private functions

	std::unique_ptr<UI> GameScene::createGameUI()
	{
		std::unique_ptr<UI> ui = std::make_unique<UI>();

		// === Game Info ===

		std::unique_ptr<UIHGroup> infoBar = std::make_unique<UIHGroup>(8.f, sf::Vector2f(0, 0));

		std::unique_ptr<Text> playerText1 = std::make_unique<Text>("JOEL");
		std::unique_ptr<Text> vsText = std::make_unique<Text>("VS");
		std::unique_ptr<Text> playerText2 = std::make_unique<Text>("JOEL");

		infoBar->addElement(std::move(playerText1));
		infoBar->addElement(std::move(vsText));
		infoBar->addElement(std::move(playerText2));
		infoBar->setPos({
			m_board->getPos().x + m_board->getSize().x / 2 - infoBar->getSize().x / 2,
			m_board->getPos().y + 0.5f * (m_board->getPadding().t - infoBar->getSize().y)
		});
		
		// === Actions ===

		std::unique_ptr<UIVGroup> actionMenu = std::make_unique<UIVGroup>(2.f, sf::Vector2f(0, 0));

		std::unique_ptr<Button> drawButton = std::make_unique<Button>("DRAW", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		drawButton->setCallback([this]() { this->drawToHand(); });
		std::unique_ptr<Button> clearButton = std::make_unique<Button>("CLEAR", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		clearButton->setCallback([this]() { this->drawToHand(); });

		actionMenu->addElement(std::move(drawButton));
		actionMenu->addElement(std::move(clearButton));
		actionMenu->setPos(m_hand->getPos() + sf::Vector2f(
			std::floor(m_hand->getSize().x - m_hand->getPadding().r + (m_hand->getPadding().r - actionMenu->getSize().x) / 2.f),
			std::floor(m_hand->getPadding().t + 0.5 * (m_hand->getSize().y - m_hand->getPadding().t - m_hand->getPadding().b - actionMenu->getSize().y)))
		);

		// ======

		ui->addElement(std::move(infoBar));
		ui->addElement(std::move(actionMenu));

		return std::move(ui);
	}

	void GameScene::drawToHand()
	{
		m_hand->drawTileFromDeck(m_game.getDeck());
	}

	// Swaps the tiles at From and To if they are valid in their destinations.
	// A tile is valid if:
	// - It has no immediate neighbours in the cardinal directions OR
	// - It forms a Meld that is:
	//   - A Set: tiles have the same rank (max one of each suit) OR
	//   - A Run: tiles form a sequence in the same direction
	// (NOT IMPLEMENTED) Jokers can be any suit or rank.
	bool GameScene::canSwap(Slot& from, Slot& to)
	{
		// Assumes that m_currentTile is the tile represented by from
		if (!m_currentTile) return false;

		// Always allow hand to hand or same slot to same slot swapping
		if (from.getParent() == m_hand.get() && to.getParent() == m_hand.get() || &from == &to) return true;

		// Rule checking lambda - always goes left to right or top to bottom
		// Returns whether a given array doesn't violates the rules.
		auto checkRules = [](std::vector<sf::Vector2i> rep) {
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

		auto initVectorRep = [sameRow, sameCol, &from, &to](
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
					vector[index] = { (*currentTile)->getSuit(), (*currentTile)->getRank() };
				}

				if (to.hasTile() && ((!isTo) || (isRow && sameRow) || (!isRow && sameCol)))
				{
					unsigned int index = isRow ? from.getCoords().x : from.getCoords().y;
					vector[index] = { to.getTile()->getSuit(), to.getTile()->getRank() };
				}
			};

		// ============ To ============
		if (to.getParent() != m_hand.get())
		{
			// ============ To row ============
			initVectorRep(toRow, &m_currentTile, true, true);
			if (!checkRules(toRow)) return false;

			// ============ To col ============
			initVectorRep(toCol, &m_currentTile, false, true);
			if (!checkRules(toCol)) return false;
		}

		// ============ From ============
		if (to.hasTile() && from.getParent() != m_hand.get())
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
	void GameScene::handleDragAndDrop(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event)
	{
		if (!event.has_value()) return;

		if (m_currentTile)
		{
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				// If right click, cancel move
				if (mouseButtonPressed->button == sf::Mouse::Button::Right)
				{
					// Deselect all slots from being highlighted
					for (auto& board : { m_board, m_hand })
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
				for (auto& board : { m_board, m_hand })
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
								return;
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

	// Constructor

	GameScene::GameScene(Game& game) : Scene(game)
	{
		// Create and position new board and hand
		m_board = std::make_shared<Board>(11u, 11u, 2, sf::Vector2f(0, 0), Padding(48, 48, 16, 8));
		m_hand = std::make_shared<Board>(11u, 2u, 2, sf::Vector2f(0, 0), Padding(48, 48, 8, 16));

		//m_board->setPos({ kInitWindowWidth / 2 - m_board->getSize().x / 2, kInitWindowHeight / 2 - m_board->getSize().y / 2 });
		m_hand->setPos({ 0, m_board->getSize().y });

		m_UI = createGameUI();
	}

	// Getters

	std::vector<WorldObject> GameScene::itemsToCentre()
	{
		// Centre based on the board and hand
		return { *m_board, *m_hand };
	}

	// Public functions

	void GameScene::handleEvents(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event)
	{
		m_UI->handleEvents(mousePosView, event);

		handleDragAndDrop(mousePosView, event);
	}

	void GameScene::update(const sf::Vector2f& mousePosView)
	{
		// Update dragged tile position
		if (m_currentTile)
		{
			m_currentTile->update(mousePosView);
		}
	}

	void GameScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Draw board
		if (m_board)
		{
			target.draw(*m_board);
		}

		// Draw hand
		if (m_hand)
		{
			target.draw(*m_hand);
		}

		// Draw dragged tile
		if (m_currentTile)
		{
			target.draw(*m_currentTile);
		}

		// Draw UI
		target.draw(*m_UI);
	}
}