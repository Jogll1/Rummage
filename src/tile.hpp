#pragma once

#include <SFML/Graphics.hpp>

namespace Rummage 
{
	enum Suit : uint8_t {
		SUIT_NONE = 0,
		SUIT_GOLD = 1,
		SUIT_SILVER = 2,
		SUIT_COPPER = 3,
		SUIT_IRON = 4,
		SUIT_MAX = 5
	};

	enum Rank : uint8_t {
		RANK_NONE = 0,
		RANK_A = 1,
		RANK_2 = 2,
		RANK_3 = 3,
		RANK_4 = 4,
		RANK_5 = 5,
		RANK_6 = 6,
		RANK_7 = 7,
		RANK_8 = 8,
		RANK_9 = 9,
		RANK_J = 10,
		RANK_Q = 11,
		RANK_K = 12,
		RANK_MAX = 13
	};

	class Tile : public sf::Drawable
	{
	private:
		const Suit m_suit;
		const Rank m_rank;

		bool m_isMoving = false;

		sf::Sprite m_sprite;
	public:
		static const unsigned int kTileSize = 18;

		Tile(Suit s = SUIT_NONE, Rank r = RANK_NONE);

		// Public functions

		// Getters

		static Suit getRandomSuit() { return Suit(rand() % 4 + 1); }
		static Rank getRandomRank() { return Rank(rand() % RANK_MAX); }

		const Suit getSuit() const { return m_suit; }
		const Rank getRank() const { return m_rank; }

		// Setters

		void setMoving(bool value) { m_isMoving = value; }
		void setTilePosition(sf::Vector2f pos) { m_sprite.setPosition(pos); }
		void setIsMoving(bool value) { m_isMoving = value; }

		void update(sf::Vector2f mousePosView = sf::Vector2f(0, 0)); 
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}