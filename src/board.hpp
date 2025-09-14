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
		unsigned int m_paddingL = 16; // Padding in pixels
		unsigned int m_paddingR = 16; 
		unsigned int m_paddingT = 16; 
		unsigned int m_paddingB = 16; 
		unsigned int m_gap = 2;       // Gap in pixels

		unsigned int m_tilesX;       // How many rows of tiles
		unsigned int m_tilesY;       // How many columns of tiles

		sf::Vector2f m_pos;          // Position of the board in pixels
		sf::Vector2f m_size;         // Size of board in pixels

		std::vector<Slot> m_slots;
		std::unique_ptr<Tile> m_currentTile = nullptr; // Only allow one tile to be moved at a time
		Slot* m_lastSlot = nullptr;                      // Default to a slot once a move is cancelled

		void setBoardPositions();
	public:
		Board(unsigned int tilesX, unsigned int tilesY, sf::Vector2f pos = {0, 0});
		virtual ~Board();

		// Getters

		unsigned int getTilesX() const { return m_tilesX; }
		unsigned int getTilesY() const { return m_tilesY; }

		sf::Vector2f getPos() const { return m_pos; }
		sf::Vector2f getCentrePos() const { return m_pos + m_size / 2.f; }
		sf::Vector2f getSize() const { return m_size; }
		Slot* getSlotAt(unsigned int x, unsigned int y);

		// Setters

		void setPos(sf::Vector2f newPos);

		// Public functions

		void handleEvents(sf::RenderWindow& window, const std::optional<sf::Event> event);
		void update(sf::Vector2f mousePosView);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}