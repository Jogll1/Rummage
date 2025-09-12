#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "tile.hpp"

namespace Rummage
{
	class Slot : public sf::Drawable
	{
	private:
		std::unique_ptr<Tile> m_tile; // hmm - maybe not use unique_ptr

		sf::Sprite m_sprite;
	public:
		Slot();
		~Slot() = default; // Automatically deletes tile

		static const unsigned int kSlotSize = 18;

		// Public functions

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void setPosition(sf::Vector2f pos);
		void setTile(std::unique_ptr<Tile> tile);
	};
}