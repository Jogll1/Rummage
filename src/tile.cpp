#include "tile.hpp"

#include <iostream>

#include "resources.hpp"

namespace Rummage 
{
	Tile::Tile(Suit s, Rank r) : m_suit(s), m_rank(r), m_sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath))
	{
		m_isMoving = false;
		m_sprite.setTextureRect(sf::IntRect({ 3 * kTileSize, 0 }, { kTileSize, kTileSize }));
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