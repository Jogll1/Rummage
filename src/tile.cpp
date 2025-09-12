#include "tile.hpp"

#include <iostream>

namespace Rummage 
{
	Tile::Tile() : rank(RANK_NONE), suit(SUIT_NONE) {}

	Tile::Tile(Suit s, Rank r) : rank(r), suit(s) 
	{
		/*if (!texture.loadFromFile("../../../../resources/images/RummageTile.png"))
		{
			exit(0);
		}*/
	}

	void Tile::drawAt(sf::RenderWindow& window, sf::Vector2f pos)
	{
		sf::RectangleShape rect(sf::Vector2f(kTileSize, kTileSize));

		rect.setPosition(pos);
		rect.setFillColor(sf::Color(233, 233, 217));

		window.draw(rect);
	}

	void Tile::move()
	{
		//sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}
}