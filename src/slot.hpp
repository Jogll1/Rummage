#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "tile.hpp"
#include "resources.hpp"

namespace Rummage
{
	class Board; // Forward declaration

	class Slot : public sf::Drawable
	{
	private:
		std::shared_ptr<sf::Shader> m_outlineShader = ResourceManager::getShader(SHADERS_PATH "outline.shader", sf::Shader::Type::Fragment);

		std::unique_ptr<Tile> m_tile;
		sf::Sprite m_sprite = sf::Sprite(*ResourceManager::getTexture(ResourceManager::kAtlasPath));

		Board* m_parent = nullptr;
		sf::Vector2u m_coords = { 0, 0 };
		bool m_drawOutline = false;
	public:
		Slot();
		~Slot() = default;

		static const unsigned int kSlotSize = 18;

		// Accessors

		sf::Vector2f getPosition() const { return m_sprite.getPosition(); }
		sf::Vector2u getCoords() const { return m_coords; }
		bool hasTile() const { return m_tile != nullptr; }
		std::unique_ptr<Tile>& getTile() { return m_tile; }
		Board* getParent() { return m_parent; }

		// Public functions

		void setCoords(sf::Vector2u coords) { m_coords = coords; }
		void setPosition(sf::Vector2f pos);
		void setParent(Board* board) { m_parent = board; }
		bool isMouseOver(const sf::Vector2f& mousePosView) const;

		void setTile(std::unique_ptr<Tile> tilePtr);
		void setOutline(bool value) { m_drawOutline = value; }

		std::unique_ptr<Tile> dropTile();
		void drawTile(sf::RenderTarget& target, sf::RenderStates states) const;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}