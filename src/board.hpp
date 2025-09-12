#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "slot.hpp"

namespace Rummage
{
	class Board : public sf::Drawable
	{
	private:
		unsigned int m_padding = 16; // Padding in pixels
		unsigned int m_gap = 2;      // Gap in pixels

		unsigned int m_tilesX;       // How many rows of tiles
		unsigned int m_tilesY;       // How many columns of tiles

		sf::Vector2f m_size;         // Size of board in pixels

		std::vector<Slot> m_slots;
	public:
		Board(unsigned int x, unsigned int y);
		virtual ~Board();

		// Getters

		unsigned int getTilesX() const;
		unsigned int getTilesY() const;

		sf::Vector2f getSize() const;
		Slot* getSlotAt(unsigned int x, unsigned int y);

		// Public functions

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}