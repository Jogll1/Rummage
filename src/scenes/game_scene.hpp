#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "scene.hpp"
#include "../ui/ui.hpp"
#include "../board.hpp"

namespace Rummage
{
	class GameScene : public Scene
	{
	private:
		std::shared_ptr<Board> m_board; // Where tiles will be played to
		std::shared_ptr<Board> m_hand;  // Where tiles will be played from

		std::unique_ptr<UI> createGameUI();

	public:
		GameScene(Game& game);

		// Getters

		std::vector<WorldObject> itemsToCentre() override;

		// Public functions

		void handleEvents(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event) override;
		void update() override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}