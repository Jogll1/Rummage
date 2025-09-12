#pragma once

#include <memory>

#include "tile.hpp"

namespace Rummage
{
	class Slot
	{
	private:
		std::unique_ptr<Tile> tile;
	public:
		Slot() = default;
		~Slot() = default; // Automatically deletes tile

		static const unsigned int kSlotSize = 18;

		// Public functions

		void drawAt(sf::RenderWindow& window, sf::Vector2f pos);
		void setTile(std::unique_ptr<Tile> tile);
	};
}