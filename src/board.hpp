#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "slot.hpp"
#include "tile.hpp"

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
		std::unique_ptr<Tile> m_currentTile = nullptr; // Only allow one tile to be moved at a time
		Slot* m_lastSlot = nullptr;                      // Default to a slot once a move is cancelled
	public:
		Board(unsigned int x, unsigned int y);
		virtual ~Board();

		// Getters

		unsigned int getTilesX() const;
		unsigned int getTilesY() const;

		sf::Vector2f getSize() const;
		Slot* getSlotAt(unsigned int x, unsigned int y);

		// Public functions

		void handleEvents(sf::RenderWindow& window, const std::optional<sf::Event> event);
		void update(sf::Vector2f mousePosView);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}