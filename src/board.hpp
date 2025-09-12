#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "slot.hpp"

namespace Rummage
{
	class Board
	{
	private:
		unsigned int padding = 16; // Padding in pixels
		unsigned int gap = 2;      // Gap in pixels

		unsigned int tilesX;       // How many rows of tiles
		unsigned int tilesY;       // How many columns of tiles

		sf::Vector2f size;         // Size of board in pixels

		std::vector<Slot> slots;
	public:
		Board(unsigned int x, unsigned int y);
		virtual ~Board();

		// Getters

		unsigned int getTilesX() const;
		unsigned int getTilesY() const;

		sf::Vector2f getSize() const;
		Slot& getSlotAt(unsigned int x, unsigned int y);

		// Public functions

		void draw(sf::RenderWindow& window);
	};
}