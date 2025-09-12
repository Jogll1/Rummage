#include "tile.hpp"

#include <iostream>

#include "resources.hpp"

namespace Rummage 
{
	Tile::Tile(Suit s, Rank r) : m_suit(s), m_rank(r), m_sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath))
	{
		m_sprite.setTextureRect(sf::IntRect({ 3 * kTileSize, 0 }, { kTileSize, kTileSize }));
	}

	// Public functions

	void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_sprite);
	}

	void Tile::move()
	{
		//sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}

	void Tile::setPosition(sf::Vector2f pos)
	{
		m_sprite.setPosition(pos);
	}
}