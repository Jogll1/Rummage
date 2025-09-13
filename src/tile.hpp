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

	class Slot; // Forward Declaration

	class Tile : public sf::Drawable
	{
	private:
		const Suit m_suit;
		const Rank m_rank;

		bool m_isMoving = false;

		sf::Sprite m_sprite;
		Slot* m_slot = nullptr;
	public:
		static const unsigned int kTileSize = 18;

		Tile(Suit s = SUIT_NONE, Rank r = RANK_NONE);

		// Public functions

		void setMoving(bool value) { m_isMoving = value; }
		void setParent(Slot* slot) { m_slot = slot; }
		void setTilePosition(sf::Vector2f pos);
		void setIsMoving(bool value);

		void update(sf::Vector2f mousePosView = sf::Vector2f(0, 0)); 
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}