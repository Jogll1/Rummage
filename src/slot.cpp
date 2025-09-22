#include "slot.hpp"

#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "resources.hpp"
#include "tile.hpp"

namespace Rummage
{
	// Constructor

	Slot::Slot() : m_tile(nullptr)
	{
		// Set sprite rect
		m_sprite.setTextureRect(sf::IntRect({0, 0}, {kSlotSize + 4, kSlotSize + 4}));
	}

	// Public functions

	void Slot::setPosition(sf::Vector2f pos)
	{
		m_sprite.setPosition(pos);

		if (m_tile)
		{
			m_tile->setTilePosition(pos);
		}
	}

	bool Slot::isMouseOver(const sf::Vector2f& mousePosView) const
	{
		// Factor in gaps in texture
		sf::FloatRect properBounds = m_sprite.getGlobalBounds();
		properBounds.position += {2, 2};
		properBounds.size -= {4, 4};

		return properBounds.contains(mousePosView);
	}

	void Slot::setTile(std::unique_ptr<Tile> tilePtr)
	{
		m_tile = std::move(tilePtr);
		m_tile->setTilePosition(m_sprite.getPosition());
	}

	std::unique_ptr<Tile> Slot::dropTile()
	{
		if (m_tile)
		{
			return std::move(m_tile);
		}

		return nullptr;
	}

	void Slot::drawTile(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_tile)
		{
			m_tile->draw(target, states);
		}
	}

	void Slot::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_drawOutline && m_outlineShader)
		{
			target.draw(m_sprite, m_outlineShader.get());
		}
		else
		{
			target.draw(m_sprite);
		}
	}
}