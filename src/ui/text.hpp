#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "ui.hpp"
#include "../resources.hpp"

namespace Rummage
{
	class Text : public UIObject
	{
	private:
		std::string m_cText = "";
		sf::Text m_text = sf::Text(*ResourceManager::getFont(RESOURCES_PATH "8bit-ascii.ttf"));
		UI::Align m_align = UI::Align::LEFT;
	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		Text(const std::string& text, unsigned int size = 100, sf::Color colour = { 40, 39, 39 }, sf::Vector2f pos = {0, 0}, Padding padding = {0, 0, 0, 0});

		// Setters

		void setText(const std::string& text);
		void setAlign(UI::Align align);
		virtual void setPos(const sf::Vector2f& newPos) override;

		// Public functions

		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) override {}
	};
}