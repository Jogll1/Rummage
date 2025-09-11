#pragma once

#include <SFML/Graphics.hpp>

namespace Rummage 
{
	class Tile: public sf::Drawable, public sf::Transformable
	{
	private:
		const int rank;
		const int suit;

		bool is_moving;

		sf::RenderWindow& window;
		sf::Texture texture;
	public:
		Tile(sf::RenderWindow& window, int rank, int suit);

		void draw();
		void move();
	};
}