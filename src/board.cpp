#include "board.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "slot.hpp"
#include "tile.hpp"

namespace Rummage
{
	// Constructor and Destructor

	Board::Board(unsigned int tilesX, unsigned int tilesY, unsigned int gap, sf::Vector2f pos, Padding padding)
		: WorldObject(pos, padding), m_slotsX(tilesX), m_slotsY(tilesY), m_gap(gap), m_slots(tilesX * tilesY)
	{
		// Width:  paddingL + (tile width + gapX) * tilesX - gapX + paddingR
		// Height: paddingT + (tile height + gapY) * tilesY - gapY + paddingB

		setPos(pos);

		m_size = sf::Vector2f(
			static_cast<float>(m_padding.l + (Tile::kTileSize + m_gap) * m_slotsX - m_gap + m_padding.r),
			static_cast<float>(m_padding.t + (Tile::kTileSize + m_gap) * m_slotsY - m_gap + m_padding.b)
		);

		// Set slot coords
		for (unsigned int y = 0; y < m_slotsY; y++) {
			for (unsigned int x = 0; x < m_slotsX; x++) {
				Slot& slot = m_slots[y * m_slotsX + x];
				slot.setCoords({x, y});
				slot.setParent(this);
			}
		}
	}

	// Getters

	// Get the slot on the board at (x, y).
	// Returns nullptr for invalid slots.
	Slot* Board::getSlotAt(unsigned int x, unsigned int y)
	{
		if (y >= 0 && y < m_slotsY && x >= 0 && x < m_slotsX)
		{
			return &m_slots.at(y * m_slotsX + x);
		}

		return nullptr;
	}

	// Setters

	void Board::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos; 
		
		// Position each slot
		for (unsigned int y = 0; y < m_slotsY; y++) {
			for (unsigned int x = 0; x < m_slotsX; x++) {
				sf::Vector2f pos = sf::Vector2f(
					static_cast<float>(m_pos.x + m_padding.l + (Tile::kTileSize + m_gap) * x - m_gap),
					static_cast<float>(m_pos.y + m_padding.t + (Tile::kTileSize + m_gap) * y - m_gap)
				);

				m_slots[y * m_slotsX + x].setPosition(pos);
			}
		}
	}

	// Public functions

	bool Board::isFull()
	{
		for (Slot& slot : m_slots)
		{
			if (!slot.hasTile())
			{
				return false;
			}
		}

		return true;
	}

	void Board::sendTileToFirstSlot(std::unique_ptr<Tile> tile)
	{
		if (tile)
		{
			for (Slot& slot : m_slots)
			{
				if (!slot.hasTile())
				{
					slot.setTile(std::move(tile));
					return;
				}
			}
		}
	}

	void Board::drawTileFromDeck(std::vector<std::unique_ptr<Tile>>& deck)
	{
		if (deck.empty() || isFull()) return;

		sendTileToFirstSlot(std::move(deck.back()));
		deck.pop_back();
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
