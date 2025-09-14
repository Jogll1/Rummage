#include "slot.hpp"

#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "resources.hpp"
#include "tile.hpp"

namespace Rummage
{
	// Constructor

	Slot::Slot() : m_tile(nullptr), m_sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath))
	{
		m_sprite.setTextureRect(sf::IntRect({0, 0}, {kSlotSize, kSlotSize}));
	}

	// Accessors

	sf::Vector2f Slot::getPosition() const
	{
		return m_sprite.getPosition();
	}

	// Public functions

	void Slot::setSlotPosition(sf::Vector2f pos)
	{
		m_sprite.setPosition(pos);

		if (m_tile)
		{
			m_tile->setTilePosition(pos);
		}
	}

	void Slot::setTile(std::unique_ptr<Tile> tilePtr)
	{
		m_tile = std::move(tilePtr);
		m_tile->setTilePosition(m_sprite.getPosition());
		m_tile->setParent(this);
	}

	bool Slot::isMouseOver(sf::Vector2f mousePosView) const
	{
		return m_sprite.getGlobalBounds().contains(mousePosView);
	}

	std::unique_ptr<Tile> Slot::dropTile()
	{
		if (m_tile)
		{
			m_tile->setParent(nullptr);
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

	void Slot::update(sf::Vector2f mousePosView)
	{
		
	}

	void Slot::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_sprite);
	}
}