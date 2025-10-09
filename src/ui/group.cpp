#include "group.hpp"

#include "ui.hpp"

namespace Rummage
{
#pragma region Group
	UIObject* UIGroup::findWithTag(const std::string tag)
	{
		for (auto& obj : m_objs)
		{
			if (UIObject* found = obj->findWithTag(tag))
			{
				return found;
			}
		}

		return nullptr;
	}

	void UIGroup::addElement(std::unique_ptr<UIObject> obj)
	{
		m_objs.push_back(std::move(obj));
		setSize();
		setPos(m_pos);
	}

	void UIGroup::handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event)
	{
		if (m_visible)
		{
			for (auto& obj : m_objs)
			{
				obj->handleEvents(mousePos, event);
			}
		}
	}

	void UIGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_visible)
		{
			for (auto& obj : m_objs)
			{
				target.draw(*obj, states);
			}
		}
	}
#pragma endregion

#pragma region V Group
	UIVGroup::UIVGroup(float gap, sf::Vector2f pos, Padding padding) : UIGroup(gap, pos, padding)
	{
		setSize();
	}

	// Setters

	void UIVGroup::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos;

		size_t spacing = 0;
		int lastI = 0;
		for (size_t i = 0; i < m_objs.size(); i++)
		{
			auto& obj = m_objs[i];

			if (obj->isVisible())
			{
				const sf::Vector2f pos(
					m_padding.l + newPos.x,
					spacing > 0 ? m_objs[lastI]->getPos().y + m_objs[lastI]->getSize().y + m_gap : m_padding.t + newPos.y
				);

				obj->setPos(pos);
				lastI = spacing;
				spacing++;
			}
		}
	}

	void UIVGroup::setSize()
	{
		float x = 0;
		float y = m_gap * (m_objs.size() - 1);

		for (auto& obj : m_objs)
		{
			if (obj->isVisible())
			{
				x = std::max(obj->getSize().x, x);
				y += obj->getSize().y;
			}
		}

		x += m_padding.l + m_padding.r;
		y += m_padding.t + m_padding.b;

		m_size = sf::Vector2f(x, y);
	}
#pragma endregion

#pragma region H Group
	UIHGroup::UIHGroup(float gap, sf::Vector2f pos, Padding padding) : UIGroup(gap, pos, padding)
	{
		setSize();
	}

	// Setters

	void UIHGroup::setPos(const sf::Vector2f& newPos)
	{
		m_pos = newPos;

		size_t spacing = 0;
		int lastI = 0;
		for (size_t i = 0; i < m_objs.size(); i++)
		{
			auto& obj = m_objs[i];

			if (obj->isVisible())
			{
				const sf::Vector2f pos(
					spacing > 0 ? m_objs[lastI]->getPos().x + m_objs[lastI]->getSize().x + m_gap : m_padding.l + newPos.x,
					m_padding.t + newPos.y
				);

				obj->setPos(pos);
				lastI = spacing;
				spacing++;
			}
		}
	}

	void UIHGroup::setSize()
	{
		float x = m_gap * (m_objs.size() - 1);
		float y = 0;

		for (auto& obj : m_objs)
		{
			if (obj->isVisible())
			{
				x += obj->getSize().x;
				y = std::max(obj->getSize().y, y);
			}
		}

		x += m_padding.l + m_padding.r;
		y += m_padding.t + m_padding.b;

		m_size = sf::Vector2f(x, y);
	}
#pragma endregion
}