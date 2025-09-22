#include "button.hpp"

namespace Rummage
{
	Button::Button(const std::string& text, sf::IntRect normalRect, sf::IntRect clickedRect, std::function<void()> onClick, sf::Vector2f pos)
		: m_onClick(onClick)
	{
		m_spriteNormal.setTextureRect(normalRect);
		m_spriteClicked.setTextureRect(clickedRect);
		m_currentSprite = &m_spriteNormal;

		// Set size accounting for blank space around sprite
		m_size = m_spriteNormal.getGlobalBounds().size;

		m_text.setString(text);
		m_text.setFillColor({ 40, 39, 39 });
		m_text.setCharacterSize(100);
		m_text.setScale({ 0.05f, 0.05f});

		setPos(pos);
	}

	// Setters

	void Button::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos;

		m_spriteNormal.setPosition(newPos);
		m_spriteClicked.setPosition(newPos);

		m_text.setPosition(getCentrePos() - m_text.getGlobalBounds().size / 2.f + sf::Vector2f(0, -1.5f));
	}

	// Public functions

	bool Button::isMouseOver(const sf::Vector2f& mousePosView) const
	{
		// Factor in gaps in texture
		sf::FloatRect properBounds = (*m_currentSprite).getGlobalBounds();

		return properBounds.contains(mousePosView);
	}

	void Button::handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event)
	{
		// Check for click
		if (isMouseOver(mousePos) && !m_isClicked)
		{
			m_drawOutline = true;

			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					// Click button
					m_currentSprite = &m_spriteClicked;
					m_isClicked = true;

					// Move text down a bit
					m_text.setPosition(getCentrePos() - m_text.getGlobalBounds().size / 2.f + sf::Vector2f(0, 1.5f));
				}
			}
		}
		else
		{
			m_drawOutline = false;
		}

		if (m_isClicked)
		{
			if (const auto* mouseButtonRelease = event->getIf<sf::Event::MouseButtonReleased>())
			{
				if (mouseButtonRelease->button == sf::Mouse::Button::Left)
				{
					m_isClicked = false;
					m_currentSprite = &m_spriteNormal;

					// Reset text pos
					m_text.setPosition(getCentrePos() - m_text.getGlobalBounds().size / 2.f + sf::Vector2f(0, -1.5f));

					// Callable
					m_onClick();
				}
			}
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