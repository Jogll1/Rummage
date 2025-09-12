#include "slot.hpp"

namespace Rummage
{
	// Public functions

	void Slot::drawAt(sf::RenderWindow& window, sf::Vector2f pos)
	{
		// Draw tile or slot
		if (tile)
		{
			tile->drawAt(window, pos);
		}
		else
		{
			sf::RectangleShape rect(sf::Vector2f(kSlotSize, kSlotSize));

			rect.setPosition(pos);
			rect.setFillColor(sf::Color(57, 158, 57));

			window.draw(rect);
		}
	}

	void Slot::setTile(std::unique_ptr<Tile> tile)
	{
		tile = std::move(tile);
	}
}