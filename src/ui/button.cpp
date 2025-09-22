#include "button.hpp"

namespace Rummage
{
	Button::Button(const std::string& text, sf::IntRect normalRect, sf::IntRect clickedRect, std::function<void()> onClick, sf::Vector2f pos)
		: m_onClick(onClick)
	{
		setPos(pos);

		m_spriteNormal.setTextureRect(normalRect);
		m_spriteClicked.setTextureRect(clickedRect);
		m_currentSprite = &m_spriteNormal;

		m_size = m_spriteNormal.getGlobalBounds().size;

		m_text.setString(text);
		m_text.setFillColor({ 40, 39, 39, 1 });
		m_text.setCharacterSize(100);
		m_text.setPosition({0, 0});
		//m_text.setPosition(getCentrePos() - m_text.getGlobalBounds().size / 2.f);
	}

	// Setters

	void Button::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos;

		m_spriteNormal.setPosition(newPos);
		m_spriteClicked.setPosition(newPos);
	}

	// Public functions

	bool Button::isMouseOver(const sf::Vector2f& mousePosView) const
	{
		// Factor in gaps in texture
		sf::FloatRect properBounds = (*m_currentSprite).getGlobalBounds();
		properBounds.position += {5, 5};
		properBounds.size -= {5, 5};

		return properBounds.contains(mousePosView);
	}

	void Button::handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event)
	{
		// Check for click
		if (isMouseOver(mousePos))
		{
			m_drawOutline = true;

			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					// Click button
					m_currentSprite = &m_spriteClicked;
					m_clickClock.restart();
					m_isClicked = true;

					m_onClick();
				}
			}
		}
		else
		{
			m_drawOutline = false;
		}

		// Timer
		if (m_isClicked && m_clickClock.getElapsedTime().asSeconds() >= m_clickTime)
		{
			m_isClicked = false;
			m_currentSprite = &m_spriteNormal;
		}
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_drawOutline && m_outlineShader)
		{
			states.shader = m_outlineShader.get();
			target.draw(*m_currentSprite, states);
		}
		else
		{
			target.draw(*m_currentSprite, states);
		}

		target.draw(m_text);
	}
}