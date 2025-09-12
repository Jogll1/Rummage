#include "slot.hpp"

#include <SFML/Graphics.hpp>

#include "resources.hpp"

namespace Rummage
{
	// Constructor

	Slot::Slot() : m_tile(nullptr), m_sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath))
	{
		m_sprite.setTextureRect(sf::IntRect({0, 0}, {kSlotSize, kSlotSize}));
	}

	// Public functions

	void Slot::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Draw tile or slot
		if (m_tile)
		{
			target.draw(*m_tile);
		}
		else
		{
			target.draw(m_sprite);
		}
	}

	void Slot::setPosition(sf::Vector2f pos)
	{
		m_sprite.setPosition(pos);
	}

	void Slot::setTile(std::unique_ptr<Tile> tilePtr)
	{
		m_tile = std::move(tilePtr);
		m_tile->setPosition(m_sprite.getPosition());
	}
}