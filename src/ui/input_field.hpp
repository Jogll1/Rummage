#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "ui.hpp"
#include "../resources.hpp"

namespace Rummage
{
	class InputField : public UIObject
	{
	private:
		std::shared_ptr<sf::Shader> m_outlineShader = ResourceManager::getShader(SHADERS_PATH "outline.shader", sf::Shader::Type::Fragment);
		bool m_drawOutline = false;

		sf::Sprite m_sprite = sf::Sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath));

		sf::String m_value = "Test";
		sf::Text m_text = sf::Text(*ResourceManager::getFont(RESOURCES_PATH "8bit-ascii.ttf"));

		/*sf::Clock m_caretClock;
		float m_caretBlinkTime = 0.5f;
		bool m_showCaret = true;*/

		unsigned int m_maxChars;
		bool m_isFocused = false;
	public:
		InputField(sf::IntRect spriteRect, unsigned int maxChars = 5, sf::Vector2f pos = { 0, 0 });

		// Getters

		sf::String getValue() const { return m_value; }

		// Setters 

		virtual void setPos(const sf::Vector2f& newPos) override;

		// Public functions

		bool isMouseOver(const sf::Vector2f& mousePosView) const;

		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) override;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}