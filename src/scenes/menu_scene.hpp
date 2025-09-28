#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "scene.hpp"
#include "../ui/ui.hpp"
#include "../world_object.hpp"

namespace Rummage
{
	class MenuScene : public Scene
	{
	private:
		void openJoinMenu();
		void closeJoinMenu();

		std::unique_ptr<UI> createMenuUI();
	public:
		MenuScene(Game& game);

		// Getters

		std::vector<WorldObject> itemsToCentre() override;

		// Public functions

		void hostGame();
		void joinGame();

		void handleEvents(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event) override;
		void update(const sf::Vector2f& mousePosView) override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}