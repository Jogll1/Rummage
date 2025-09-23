#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "slot.hpp"
#include "tile.hpp"
#include "world_object.hpp"

namespace Rummage
{
	class Board : public WorldObject, public sf::Drawable
	{
	private:
		unsigned int m_gap;    // Gap in pixels

		unsigned int m_slotsX; // How many tiles in the board
		unsigned int m_slotsY; // How many columns of tiles

		std::vector<Slot> m_slots;
	public:
		Board(unsigned int tilesX, unsigned int tilesY, unsigned int gap = 2, sf::Vector2f pos = { 0, 0 }, Padding padding = { 10, 10, 10, 10 });
		virtual ~Board() = default;

		// Getters

		unsigned int getSlotsX() const { return m_slotsX; }
		unsigned int getSlotsY() const { return m_slotsY; }

		std::vector<Slot>* getSlots() { return &m_slots; }
		Slot* getSlotAt(unsigned int x, unsigned int y);

		// Setters

		virtual void setPos(const sf::Vector2f& newPos) override;

		// Public functions

		bool isFull();
		void sendTileToFirstSlot(std::unique_ptr<Tile> tile);
		void drawTileFromDeck(std::vector<std::unique_ptr<Tile>>& deck);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}