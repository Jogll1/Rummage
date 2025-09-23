#pragma once

#include <SFML/Graphics.hpp>

#include "ui.hpp"
#include "../resources.hpp"

namespace Rummage
{
	class UIImage : public UIObject
	{
	private:
		std::shared_ptr<sf::Shader> m_outlineShader = ResourceManager::getShader(SHADERS_PATH "outline.shader", sf::Shader::Type::Fragment);
		bool m_drawOutline = false;

		sf::Sprite m_sprite = sf::Sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath));
	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		UIImage(sf::IntRect spriteRect, sf::Vector2f pos = { 0, 0 }, Padding padding = { 0, 0, 0, 0 });

		// Setters

		void setOutline(bool value) { m_drawOutline = value; }
		virtual void handleEvents(const sf::Vector2f& mousePos, const std::optional<sf::Event> event) override {}
		virtual void setPos(const sf::Vector2f& newPos) override;
	};
}