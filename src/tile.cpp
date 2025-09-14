#include "tile.hpp"

#include <iostream>

#include "resources.hpp"

namespace Rummage 
{
	Tile::Tile(Suit s, Rank r) : m_suit(s), m_rank(r), m_sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath))
	{
		m_isMoving = false;

		// Set texture based on suit and rank
		int texX = 0;
		int texY = 0;

		if (m_suit == SUIT_NONE)
		{
			texX = 3 * kTileSize;
		}
		else
		{
			texX = m_rank * kTileSize;
			texY = (1 + m_suit) * kTileSize;
		}

		m_sprite.setTextureRect(sf::IntRect({ texX, texY }, { kTileSize, kTileSize }));
	}

	Suit Tile::getRandomSuit()
	{
		// Can't be none
		return Suit(rand() % 5 + 1);
	}

	Rank Tile::getRandomRank()
	{
		return Rank(rand() % 13);
	}

	// Public functions

	void Tile::setTilePosition(sf::Vector2f pos)
	{
		m_sprite.setPosition(pos);
	}

	void Tile::setIsMoving(bool value)
	{
		m_isMoving = value;
	}

	void Tile::update(sf::Vector2f mousePosView)
	{
		if (m_isMoving)
		{
			sf::Vector2f size = m_sprite.getGlobalBounds().size;
			m_sprite.setPosition(mousePosView - size / 2.f);
		}
	}

	void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_sprite, states);
	}
}