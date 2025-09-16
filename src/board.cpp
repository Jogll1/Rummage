#include "board.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "slot.hpp"
#include "tile.hpp"

namespace Rummage
{
	// Constructor and Destructor

	Board::Board(unsigned int tilesX, unsigned int tilesY, Padding padding, unsigned int gap, sf::Vector2f pos)
		: m_tilesX(tilesX), m_tilesY(tilesY), m_padding(padding), m_gap(gap), m_slots(tilesX* tilesY)
	{
		// Width:  paddingL + (tile width + gapX) * tilesX - gapX + paddingR
		// Height: paddingT + (tile height + gapY) * tilesY - gapY + paddingB

		setPos(pos);

		m_size = sf::Vector2f(
			static_cast<float>(m_padding.l + (Tile::kTileSize + m_gap) * m_tilesX - m_gap + m_padding.r),
			static_cast<float>(m_padding.t + (Tile::kTileSize + m_gap) * m_tilesY - m_gap + m_padding.b)
		);
	}

	Board::~Board() {}

	// Getters

	std::vector<Slot>* Board::getSlots()
	{
		return &m_slots;
	}

	// Get the slot on the board at (x, y).
	// Returns nullptr for invalid slots.
	Slot* Board::getSlotAt(unsigned int x, unsigned int y)
	{
		if (y >= 0 && y < m_tilesY && x >= 0 && x < m_tilesX)
		{
			return &m_slots.at(y * m_tilesX + x);
		}

		return nullptr;
	}

	// Setters

	void Board::setPos(sf::Vector2f newPos)
	{
		m_pos = newPos; 
		
		// Position each slot
		for (unsigned int y = 0; y < m_tilesY; y++) {
			for (unsigned int x = 0; x < m_tilesX; x++) {
				sf::Vector2f pos = sf::Vector2f(
					static_cast<float>(m_pos.x + m_padding.l + (Tile::kTileSize + m_gap) * x),
					static_cast<float>(m_pos.y + m_padding.t + (Tile::kTileSize + m_gap) * y)
				);

				m_slots[y * m_tilesX + x].setSlotPosition(pos);
			}
		}
	}

	// Public functions

	bool Board::isFull()
	{
		for (Slot& slot : m_slots)
		{
			if (slot.hasTile())
			{
				return false;
			}
		}

		return true;
	}

	void Board::drawTileFromDeck(std::vector<std::unique_ptr<Tile>>& deck)
	{
		for (Slot& slot : m_slots)
		{
			if (!slot.hasTile() && !deck.empty())
			{
				slot.setTile(std::move(deck.back()));
				deck.pop_back();
				return;
			}
		}
	}

	void Board::update(sf::Vector2f mousePosView, bool hasCurrentTile)
	{
		for (Slot& slot : m_slots)
		{
			slot.setOutline(slot.isMouseOver(mousePosView) && hasCurrentTile);
		}
	}

	void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const 
	{
		// Draw slots
		for (const Slot& slot : m_slots)
		{
			target.draw(slot);
		}

		// Draw tiles in slots
		for (const Slot& slot : m_slots)
		{
			slot.drawTile(target, states);
		}
	}
}
