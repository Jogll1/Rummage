#include "image.hpp"

#include "ui.hpp"

namespace Rummage
{
	UIImage::UIImage(sf::IntRect spriteRect, sf::Vector2f pos, Padding padding) : UIObject(pos, padding)
	{
		m_sprite.setTextureRect(spriteRect);
		m_size = m_sprite.getGlobalBounds().size;
		setPos(pos);
	}

	// Setters

	void UIImage::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos;
		m_sprite.setPosition(newPos);
	}

	// Public functions

	void UIImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible)
		{
			if (m_drawOutline && m_outlineShader)
			{
				states.shader = m_outlineShader.get();
				target.draw(m_sprite, states);
			}
			else
			{
				target.draw(m_sprite, states);
			}
		}
	}
}