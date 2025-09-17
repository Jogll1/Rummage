#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "tile.hpp"

namespace Rummage
{
	class Slot : public sf::Drawable
	{
	private:
		sf::Shader* m_outlineShader;

		std::unique_ptr<Tile> m_tile;
		sf::Sprite m_sprite;

		bool m_drawOutline = false;
	public:
		Slot();
		~Slot() = default;

		static const unsigned int kSlotSize = 18;

		// Accessors

		sf::Vector2f getPosition() const { return m_sprite.getPosition(); }
		bool hasTile() const { return m_tile != nullptr; }
		std::unique_ptr<Tile>* getTile() { return &m_tile; }

		// Public functions

		void setSlotPosition(sf::Vector2f pos);
		bool isMouseOver(sf::Vector2f mousePosView) const;

		void setTile(std::unique_ptr<Tile> tilePtr);
		std::unique_ptr<Tile> dropTile();
		void setOutline(bool value) { m_drawOutline = value; }
		void drawTile(sf::RenderTarget& target, sf::RenderStates states) const;

		void update(sf::Vector2f mousePosView);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}