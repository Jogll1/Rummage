#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../world_object.hpp"

namespace Rummage
{
	class UIObject : public WorldObject, public sf::Drawable
	{
	protected:
		virtual void setPos(const sf::Vector2f& newPos) override = 0;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	public:
		UIObject() = default;
		virtual ~UIObject() = default;

		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) = 0;
	};

	class UI : public sf::Drawable
	{
	private:
		std::vector<std::unique_ptr<UIObject>> m_elements;
	public:
		UI() = default;
		~UI() = default;

		// Getters

		const std::vector<std::unique_ptr<UIObject>>& getElements() const { return m_elements; }

		// Public functions

		void addElement(std::unique_ptr<UIObject> obj) { m_elements.push_back(std::move(obj)); }

		void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event)
		{
			for (const auto& elem : m_elements)
			{
				elem->handleEvents(mousePos, event);
			}
		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override 
		{
			for (const auto& elem : m_elements)
			{
				target.draw(*elem, states);
			}
		}
	};
}