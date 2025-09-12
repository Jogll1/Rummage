#pragma once

#include <SFML/Graphics.hpp>

namespace Rummage 
{
	enum Suit {
		SUIT_SUN,
		SUIT_MOON,
		SUIT_VENUS,
		SUIT_MARS,
		SUIT_NONE
	};

	enum Rank {
		RANK_A,
		RANK_2,
		RANK_3,
		RANK_4,
		RANK_5,
		RANK_6,
		RANK_7,
		RANK_8,
		RANK_9,
		RANK_J,
		RANK_Q,
		RANK_K,
		RANK_NONE
	};

	class Tile
	{
	private:
		const Suit suit;
		const Rank rank;

		bool isMoving = false;

		//sf::Texture texture;
	public:
		Tile();
		Tile(Suit s, Rank r);

		static const unsigned int kTileSize = 18;

		void drawAt(sf::RenderWindow& window, sf::Vector2f pos);
		void move();
	};
}