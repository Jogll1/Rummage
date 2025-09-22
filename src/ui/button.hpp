#pragma once

#include <functional>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "../resources.hpp"
#include "ui.hpp"

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

		sf::Text m_text = sf::Text(*ResourceManager::getFont(RESOURCES_PATH "arial.ttf"));

		sf::Clock m_clickClock;
		float m_clickTime = 0.25f;
		bool m_isClicked = false;

		std::function<void()> m_onClick = nullptr;
	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		Button(const std::string& text, sf::IntRect normalRect, sf::IntRect clickedRect, std::function<void()> onClick, sf::Vector2f pos = { 0, 0 });

		// Setters

		virtual void setPos(const sf::Vector2f& newPos) override;

		// Public functions

		bool Button::isMouseOver(const sf::Vector2f& mousePosView) const;
		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) override;
	};

	class ButtonGroup : public UIObject
	{
	private:
		std::vector<std::unique_ptr<Button>> m_buttons;
		
		float m_vGap = 4.f;
	public:
		ButtonGroup() = default;
		~ButtonGroup() = default;

		virtual void setPos(const sf::Vector2f& newPos) override {
			for (size_t i = 0; i < m_buttons.size(); i++)
			{
				auto& button = m_buttons[i];
				const sf::Vector2f buttonPos(
					newPos.x,
					i > 0 ? newPos.y + m_vGap + button->getSize().y : newPos.y
				);

				button->setPos(buttonPos);
			}
		}

		void setVGap(float newGap) { m_vGap = newGap; }
		void addElement(std::unique_ptr<Button> button) { m_buttons.push_back(std::move(button)); }

		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) override
		{
			for (auto& button : m_buttons)
			{
				button->handleEvents(mousePos, event);
			}
		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			for (auto& button : m_buttons)
			{
				target.draw(*button, states);
			}
		}
	};
}