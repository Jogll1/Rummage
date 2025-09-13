#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace Rummage
{
	class Tile; // Forward declaration

	class Slot : public sf::Drawable
	{
	private:
		std::unique_ptr<Tile> m_tile;

		sf::Sprite m_sprite;
	public:
		Slot();
		~Slot() = default;

		static const unsigned int kSlotSize = 18;

		// Accessors

		sf::Vector2f getPosition() const;
		bool hasTile() const { return m_tile != nullptr; }

		// Public functions

		void setSlotPosition(sf::Vector2f pos);
		bool isMouseOver(sf::Vector2f mousePosView) const;

		void setTile(std::unique_ptr<Tile> tilePtr);
		std::unique_ptr<Tile> dropTile();
		void drawTile(sf::RenderTarget& target, sf::RenderStates states) const;

		void update(sf::Vector2f mousePosView);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}