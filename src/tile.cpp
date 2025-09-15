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
			texX = 3 * (kTileSize + 4);
		}
		else
		{
			texX = m_rank * (kTileSize + 4);
			texY = m_suit * (kTileSize + 4);
		}

		m_sprite.setTextureRect(sf::IntRect({ texX, texY }, { (kTileSize + 4), (kTileSize + 4) }));
	}

	// Public functions

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