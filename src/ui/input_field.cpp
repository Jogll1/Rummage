#include "input_field.hpp"
#include "../utils.hpp"

namespace Rummage
{
	InputField::InputField(sf::IntRect spriteRect, unsigned int maxChars, sf::Vector2f pos) : UIObject(pos), m_maxChars(maxChars)
	{
		m_sprite.setTextureRect(spriteRect);

		m_value = sf::String();
		m_size = m_sprite.getGlobalBounds().size + sf::Vector2f(m_padding.l + m_padding.r, m_padding.t + m_padding.b);

		m_text.setString(m_value);
		m_text.setFillColor({ 40, 39, 39 });
		m_text.setCharacterSize(100);
		m_text.setScale({ 0.05f, 0.05f });

		setPos(pos);
	}

	// Setters

	void InputField::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos;
		m_sprite.setPosition(newPos + sf::Vector2f(m_padding.l, m_padding.t));

		// Always align text to right
		sf::Vector2f textSize = m_text.getGlobalBounds().size;
		m_text.setPosition(roundVector(
			getCentrePos() - sf::Vector2f(getSize().x / 2.f - 4.f, textSize.y / 2.f + 0.5f) + sf::Vector2f(m_padding.l, m_padding.t)
		));
	}
	
	// Public functions

	bool InputField::isMouseOver(const sf::Vector2f& mousePosView) const
	{
		// Factor in gaps in texture
		sf::FloatRect properBounds = m_sprite.getGlobalBounds();

		return properBounds.contains(mousePosView);
	}

	void InputField::handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event)
	{
		if (visible)
		{
			// Set outline
			bool mouseOver = isMouseOver(mousePos);
			m_drawOutline = mouseOver || m_isFocused;

			// Set focus if mouse is clicked over this
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				m_isFocused = mouseOver;
			}

			if (m_isFocused)
			{
				m_drawOutline = true;

				// Look for keyboard text input
				if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
				{
					auto unicode = textEntered->unicode;

					// Remove character if backscape
					if (m_value.getSize() > 0 && unicode == 0x08)
					{
						m_value.erase(m_value.getSize() - 1);
					}

					if (m_value.getSize() < m_maxChars)
					{
						// Add unicode text
						if (unicode >= 0x30 && unicode <= 0x39 || // Nums 
							unicode >= 0x41 && unicode <= 0x5A || // Caps
							unicode >= 0x61 && unicode <= 0x7A)   // Letters
						{
							m_value += sf::String(unicode);
						}
					}

					// Update text
					m_text.setString(m_value);
					sf::Vector2f textSize = m_text.getGlobalBounds().size;
					m_text.setPosition(getCentrePos() - sf::Vector2f(getSize().x / 2.f - 4.f, textSize.y / 2.f + 0.5f));
				}
			}
		}
	}

	void InputField::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

			target.draw(m_text);
		}
	}
}