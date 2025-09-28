#pragma once

#include <functional>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "ui.hpp"
#include "../resources.hpp"

namespace Rummage
{
	class Button : public UIObject
	{
	private:
		std::shared_ptr<sf::Shader> m_outlineShader = ResourceManager::getShader(SHADERS_PATH "outline.shader", sf::Shader::Type::Fragment);
		bool m_drawOutline = false;

		sf::Sprite m_spriteNormal = sf::Sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath));
		sf::Sprite m_spriteClicked = sf::Sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath));
		sf::Sprite* m_currentSprite;

		sf::Text m_text = sf::Text(*ResourceManager::getFont(RESOURCES_PATH "8bit-ascii.ttf"));

		bool m_isClicked = false;
		std::function<void()> m_onClick = nullptr;
	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		Button(const std::string& text, sf::IntRect normalRect, sf::IntRect clickedRect, sf::Vector2f pos = { 0, 0 });

		bool isActive = true;
		bool disableOnClick = false;

		// Setters

		void setCallback(std::function<void()> onClick);
		virtual void setPos(const sf::Vector2f& newPos) override;

		// Public functions

		bool Button::isMouseOver(const sf::Vector2f& mousePosView) const;
		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) override;
	};
}