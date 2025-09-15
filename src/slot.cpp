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
		// Load sprite
		m_sprite.setTextureRect(sf::IntRect({0, 0}, {kSlotSize + 4, kSlotSize + 4}));

		// Load shader
		m_outlineShader = ResourceManager::getShader(SHADERS_PATH "highlight.shader", sf::Shader::Type::Fragment);

		// Set shader uniforms
		m_outlineShader->setUniform("texture", m_sprite.getTexture());
		m_outlineShader->setUniform("colour", sf::Glsl::Vec4 {1.0, 1.0, 1.0, 1.0});
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

	bool Slot::isMouseOver(sf::Vector2f mousePosView) const
	{
		// Factor in gaps in texture
		sf::FloatRect properBounds = m_sprite.getGlobalBounds();
		properBounds.position += {2, 2};
		properBounds.size -= {2, 2};

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

	void Slot::update(sf::Vector2f mousePosView)
	{
		
	}

	void Slot::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_drawOutline && m_outlineShader)
		{
			target.draw(m_sprite, m_outlineShader);
		}
		else
		{
			target.draw(m_sprite);
		}
	}
}