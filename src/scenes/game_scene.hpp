#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "scene.hpp"
#include "../ui/ui.hpp"
#include "../board.hpp"
#include "../slot.hpp"
#include "../tile.hpp"

namespace Rummage
{
	class GameScene : public Scene
	{
	private:
		std::shared_ptr<Board> m_board;                // Where tiles will be played to
		std::shared_ptr<Board> m_hand;                 // Where tiles will be played from

		std::unique_ptr<Tile> m_currentTile = nullptr; // Only allow one tile to be moved at a time
		Slot* m_lastSlot = nullptr;                    // Default to a slot once a move is cancelled

		// Private functions

		std::unique_ptr<UI> createGameUI();

		void drawToHand();
		bool canSwap(Slot& from, Slot& to);
		void handleDragAndDrop(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event);
	public:
		GameScene(Game& game);

		// Getters

		std::vector<WorldObject> itemsToCentre() override;

		// Public functions

		void handleEvents(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event) override;
		void update(const sf::Vector2f& mousePosView) override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}