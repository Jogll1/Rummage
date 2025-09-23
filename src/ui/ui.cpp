#include "ui.hpp"
#include "ui.hpp"
#include "ui.hpp"
#include "ui.hpp"

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../world_object.hpp"

namespace Rummage
{
#pragma region UIVGroup
	UIVGroup::UIVGroup(float gap, sf::Vector2f pos, Padding padding) : UIObject(pos, padding), m_vGap(gap)
	{
		setSize();
	}
	
	// Setters

	void UIVGroup::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos;

		size_t spacing = 0;
		for (size_t i = 0; i < m_objs.size(); i++)
		{
			auto& obj = m_objs[i];

			if (obj->m_visible)
			{
				const sf::Vector2f pos(
					m_padding.l + newPos.x,
					m_padding.t + (spacing > 0 ? newPos.y + (m_vGap + obj->getSize().y) * spacing : newPos.y)
				);

				obj->setPos(pos);
				spacing++;
			}
		}
	}

	void UIVGroup::setSize()
	{
		float x = 0;
		float y = m_vGap * (m_objs.size() - 1);

		for (auto& obj : m_objs)
		{
			if (obj->m_visible)
			{
				x = std::max(obj->getSize().x, x);
				y += obj->getSize().y;
			}
		}

		x += m_padding.l + m_padding.r;
		y += m_padding.t + m_padding.b;

		m_size = sf::Vector2f(x, y);
	}

	// Public functions

	void UIVGroup::addElement(std::unique_ptr<UIObject> obj)
	{
		m_objs.push_back(std::move(obj));
		setSize();
		setPos(m_pos);
	}

	void UIVGroup::handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event)
	{
		if (visible)
		{
			for (auto& obj : m_objs)
			{
				obj->handleEvents(mousePos, event);
			}
		}
	}

	void UIVGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible)
		{
			for (auto& obj : m_objs)
			{
				target.draw(*obj, states);
			}
		}
	}

#pragma endregion

#pragma region UI

#pragma endregion
}