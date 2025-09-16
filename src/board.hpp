#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "slot.hpp"
#include "tile.hpp"
#include "world_object.hpp"

namespace Rummage
{
	struct Padding
	{
		unsigned int l;
		unsigned int r;
		unsigned int t;
		unsigned int b;
	};

	class Board : public WorldObject, public sf::Drawable
	{
	private:
		Padding m_padding;     // Padding in pixels
		unsigned int m_gap;    // Gap in pixels

		unsigned int m_tilesX; // How many rows of tiles
		unsigned int m_tilesY; // How many columns of tiles

		std::vector<Slot> m_slots;
	public:
		Board(unsigned int tilesX, unsigned int tilesY, Padding padding = { 10, 10, 10, 10 },
			  unsigned int gap = 2, sf::Vector2f pos = {0, 0}
		);
		virtual ~Board();

		// Getters

		unsigned int getTilesX() const { return m_tilesX; }
		unsigned int getTilesY() const { return m_tilesY; }

		std::vector<Slot>* getSlots();
		Slot* getSlotAt(unsigned int x, unsigned int y);

		// Setters

		virtual void setPos(sf::Vector2f newPos) override;

		// Public functions

		bool isFull();
		void drawTileFromDeck(std::vector<std::unique_ptr<Tile>>& deck);

		void update(sf::Vector2f mousePosView, bool hasCurrentTile);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}