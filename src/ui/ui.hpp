#pragma once

#include <vector>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "../world_object.hpp"

namespace Rummage
{
	// Should probably make a set size override
	// Size includes padding?
	// Position is the top left including padding (for most of them).

	class UIObject : public WorldObject, public sf::Drawable
	{
	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	public:
		UIObject(sf::Vector2f pos = { 0, 0 }, Padding padding = { 0, 0, 0, 0 }) : WorldObject(pos, padding) {}
		virtual ~UIObject() = default;

		bool visible = true;
		std::string tag = "";
	
		virtual void setPos(const sf::Vector2f& newPos) override = 0;
		virtual UIObject* findWithTag(const std::string tag) { return tag == tag ? this : nullptr; };
		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) = 0;
	};

	class UI : public sf::Drawable
	{
	private:
		std::vector<std::unique_ptr<UIObject>> m_elements;
	public:
		enum class Align
		{
			LEFT,
			CENTER,
			RIGHT
		};

		UI() = default;
		~UI() = default;

		// Getters

		const std::vector<std::unique_ptr<UIObject>>& getElements() const { return m_elements; }

		// Public functions

		void addElement(std::unique_ptr<UIObject> obj) { m_elements.push_back(std::move(obj)); }

		UIObject* findWithTag(const std::string tag)
		{
			for (const auto& elem : m_elements)
			{
				if (UIObject* found = elem->findWithTag(tag))
				{
					return found;
				}
			}

			return nullptr;
		}

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