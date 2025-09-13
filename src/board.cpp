#include "board.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

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

				m_slots[y * m_tilesX + x].setSlotPosition(pos);
			}
		}
		
		// Test
		if (Slot* slot = getSlotAt(1, 0))
		{
			slot->setTile(std::make_unique<Tile>());
		}
		if (Slot* slot = getSlotAt(3, 2))
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

	void Board::handleEvents(sf::RenderWindow& window, const std::optional<sf::Event> event)
	{
		if (!event.has_value()) return;

		sf::Vector2f mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		if (m_currentTile)
		{
			if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
			{
				bool placed = false;
				for (Slot& slot : m_slots)
				{
					if (slot.isMouseOver(mousePosView) && !slot.hasTile())
					{
						// Drop current tile
						m_currentTile->setIsMoving(false);
						slot.setTile(std::move(m_currentTile));
						m_lastSlot = &slot;
						placed = true;

						break;
					}
				}

				if (!placed && m_lastSlot)
				{
					// Return current tile to last slot if not dropped
					m_currentTile->setIsMoving(false);
					m_lastSlot->setTile(std::move(m_currentTile));
				}
			}
		}
		else
		{
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				for (Slot& slot : m_slots)
				{
					if (slot.isMouseOver(mousePosView) && slot.hasTile())
					{
						// Pick up tile
						m_lastSlot = &slot;
						m_currentTile = slot.dropTile();
						m_currentTile->setIsMoving(true);

						break;
					}
				}
			}
		}
	}

	void Board::update(sf::Vector2f mousePosView)
	{
		if (m_currentTile)
		{
			m_currentTile->update(mousePosView);
		}
	}

	void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const 
	{
		// Draw slots
		for (const Slot& slot : m_slots)
		{
			target.draw(slot);
		}

		// Draw tiles
		for (const Slot& slot : m_slots)
		{
			slot.drawTile(target, states);
		}

		if (m_currentTile)
		{
			target.draw(*m_currentTile);
		}
	}
}
