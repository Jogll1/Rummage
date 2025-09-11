#include "tile.hpp"

namespace Rummage 
{
	Tile::Tile(sf::RenderWindow& windowRef, int rank, int suit) : window(windowRef), rank(rank), suit(suit) 
	{
		if (!this->texture.loadFromFile("../../../../resources/images/Background.png"))
		{
			exit(0);
		}

		this->is_moving = false;
	}

	void Tile::draw()
	{

	}

	void Tile::move()
	{

	}
}