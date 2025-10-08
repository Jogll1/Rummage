#include "text.hpp"

#include "ui.hpp"
#include "../utils.hpp"

namespace Rummage
{
	// Constructor

	Text::Text(const std::string& text, unsigned int size, sf::Color colour, sf::Vector2f pos, Padding padding) 
		: UIObject(pos, padding), m_cText(text)
	{
		m_text.setString(text);
		m_text.setFillColor(colour);

		// NOTE: SIZE = 100 and SCALE = { 0.05f, 0.05f} IS PIXEL SIZE
		m_text.setCharacterSize(size);
		m_text.setScale({ 0.05f, 0.05f });

		m_size = m_text.getGlobalBounds().size + sf::Vector2f(m_padding.l + m_padding.r, m_padding.t + m_padding.b);
		setPos(pos);
	}

	// Setters

	void Text::setText(const std::string& text)
	{
		m_text.setString(text);

		// Update size and position
		m_size = m_text.getGlobalBounds().size + sf::Vector2f(m_padding.l + m_padding.r, m_padding.t + m_padding.b);
		//centreText();
	}

	void Text::setAlign(UI::Align align)
	{
		m_align = align;
		setPos(m_pos);
	}

	void Text::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos;

		switch (m_align)
		{
		case UI::Align::LEFT: 
			m_text.setPosition(roundVector(newPos));
			break;
		case UI::Align::CENTER:
			m_text.setPosition(roundVector(getCentrePos() - m_size / 2.f + sf::Vector2f(0, -1.5f) + sf::Vector2f(m_padding.l, m_padding.t)));
			break;
		case UI::Align::RIGHT: 
			m_text.setPosition(roundVector(newPos - sf::Vector2f(m_size.x, 0)));
			break;
		}
		
	}

	// Public functions

	void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible)
		{
			target.draw(m_text, states);
		}
	}
}