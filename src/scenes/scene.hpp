#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../ui/ui.hpp"
#include "../world_object.hpp"

namespace Rummage
{
	class Game; // Forward declaration

	class Scene : public sf::Drawable
	{
	protected:
		std::string m_name = "Scene";

		Game& m_game;
		std::unique_ptr<UI> m_UI = nullptr;
	public:
		Scene(Game& game) : m_game(game) {}
		virtual ~Scene() = default;

		// Getters

		std::string getName() { return m_name; }
		virtual std::vector<WorldObject> itemsToCentre() = 0; // Select the WorldObjects that will centre the scene
		UI* getUI() { return m_UI.get(); }

		// Public functions

		virtual void handleEvents(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event) = 0;
		virtual void update(const sf::Vector2f& mousePosView) = 0;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	};
}