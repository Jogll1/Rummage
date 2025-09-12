#include "board.hpp"

#include <SFML/Graphics.hpp>

#include <memory>

#include "slot.hpp"
#include "tile.hpp"

namespace Rummage
{
	Board::Board(unsigned int x, unsigned int y) : m_tilesX(x), m_tilesY(y), m_slots(x * y)
	{
		// Width:  paddingL + (tile width + gapX) * tilesX - gapX + paddingR
		// Height: paddingT + (tile height + gapY) * tilesY - gapY + paddingB

		m_size = sf::Vector2f(
			static_cast<float>(m_padding + (18 + m_gap) * m_tilesX - m_gap + m_padding),
			static_cast<float>(m_padding + (18 + m_gap) * m_tilesY - m_gap + m_padding)
		);

		// Position each slot
		for (unsigned int y = 0; y < m_tilesY; y++) {
			for (unsigned int x = 0; x < m_tilesX; x++) {
				sf::Vector2f pos = sf::Vector2f(
					static_cast<float>(m_padding + (18 + m_gap) * x),
					static_cast<float>(m_padding + (18 + m_gap) * y)
				);

				m_slots[y * m_tilesX + x].setPosition(pos);
			}
		}
		
		// Test
		if (Slot* slot = getSlotAt(1, 0))
		{
			slot->setTile(std::make_unique<Tile>());
		}
	}

	Board::~Board() {}

	// Getters

	unsigned int Board::getTilesX() const
	{
		return m_tilesX;
	}

	unsigned int Board::getTilesY() const
	{
		return m_tilesY;
	}

	sf::Vector2f Board::getSize() const
	{
		return m_size;
	}

	// Get the slot on the board at (x, y).
	// Returns nullptr for invalid slots.
	Slot* Board::getSlotAt(unsigned int x, unsigned int y)
	{
		if (y >= 0 && y < m_tilesY && x >= 0 && x < m_tilesX)
		{
			return &m_slots[y * m_tilesX + x];
		}

		return nullptr;
	}

	// Public functions

	void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const 
	{
		for (const auto& slot : m_slots)
		{
			target.draw(slot, states);
		}
	}
}
