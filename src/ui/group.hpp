#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "ui.hpp"
#include "../resources.hpp"

namespace Rummage
{
	class UIGroup : public UIObject
	{
	protected:
		std::vector<std::unique_ptr<UIObject>> m_objs;

		float m_gap = 0.f;
	public:
		UIGroup(float gap, sf::Vector2f pos = { 0, 0 }, Padding padding = { 0, 0, 0, 0 }) : UIObject(pos, padding), m_gap(gap) {}
		~UIGroup() = default;

		// Setters

		virtual void setPos(const sf::Vector2f& newPos) override = 0;
		virtual void setSize() = 0;
		void setGap(float newGap) { m_gap = newGap; }

		// Public functions

		virtual UIObject* findWithTag(const std::string tag) override;

		void addElement(std::unique_ptr<UIObject> obj);

		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) override;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};

	class UIVGroup : public UIGroup
	{
	public:
		UIVGroup(float gap, sf::Vector2f pos = { 0, 0 }, Padding padding = { 0, 0, 0, 0 });
		~UIVGroup() = default;

		// Setters

		virtual void setPos(const sf::Vector2f& newPos) override;
		virtual void setSize() override;
	};

	class UIHGroup : public UIGroup
	{
	public:
		UIHGroup(float gap, sf::Vector2f pos = { 0, 0 }, Padding padding = { 0, 0, 0, 0 });
		~UIHGroup() = default;

		// Setters

		virtual void setPos(const sf::Vector2f& newPos) override;
		virtual void setSize() override;
	};
}