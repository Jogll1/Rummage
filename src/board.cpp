#include "board.hpp"

#include <SFML/Graphics.hpp>

#include "slot.hpp"

namespace Rummage
{
	Board::Board(unsigned int x, unsigned int y) : tilesX(x), tilesY(y), slots(tilesX * tilesY)
	{
		// Width:  paddingL + (tile width + gapX) * tilesX - gapX + paddingR
		// Height: paddingT + (tile height + gapY) * tilesY - gapY + paddingB

		size = sf::Vector2f(
			static_cast<float>(padding + (18 + gap) * tilesX - gap + padding),
			static_cast<float>(padding + (18 + gap) * tilesY - gap + padding)
		);
	}

	Board::~Board() {}


	// Getters

	unsigned int Board::getTilesX() const
	{
		return tilesX;
	}

	unsigned int Board::getTilesY() const
	{
		return tilesY;
	}

	sf::Vector2f Board::getSize() const
	{
		return size;
	}

	Slot& Board::getSlotAt(unsigned int x, unsigned int y)
	{
		// Get the slot on the board at (x, y)
		return slots[y * tilesX + x];
	}

	// Public functions

	void Board::draw(sf::RenderWindow& window)
	{
		for (unsigned int y = 0; y < tilesY; y++) {
			for (unsigned int x = 0; x < tilesX; x++) {
				sf::Vector2f pos = sf::Vector2f(
					static_cast<float>(padding + (18 + gap) * x),
					static_cast<float>(padding + (18 + gap) * y)
				);

				getSlotAt(x, y).drawAt(window, pos);
			}
		}
	}
}
