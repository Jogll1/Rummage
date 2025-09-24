#include "game_scene.hpp"

#include <memory>

#include "../game.hpp"
#include "../resources.hpp"

#include "../ui/ui.hpp"
#include "../ui/button.hpp"
#include "../ui/input_field.hpp"
#include "../ui/image.hpp"

namespace Rummage
{
	// Private functions

	std::unique_ptr<UI> GameScene::createGameUI()
	{
		std::unique_ptr<UI> ui = std::make_unique<UI>();

		std::unique_ptr<UIVGroup> actionMenu = std::make_unique<UIVGroup>(2.f, sf::Vector2f(0, 0));

		std::unique_ptr<Button> drawButton = std::make_unique<Button>("DRAW", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		//drawButton->setCallback([this]() { this->m_game.drawToHand(); });
		std::unique_ptr<Button> clearButton = std::make_unique<Button>("CLEAR", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		//clearButton->setCallback([this]() { this->m_game.drawToHand(); });

		actionMenu->addElement(std::move(drawButton));
		actionMenu->addElement(std::move(clearButton));
		/*actionMenu->setPos(m_hand->getPos() + sf::Vector2f(
			std::floor(m_hand->getSize().x - m_hand->getPadding().r + (m_hand->getPadding().r - actionMenu->getSize().x) / 2.f),
			std::floor((m_hand->getSize().y - actionMenu->getSize().y) / 2.f))
		);*/

		ui->addElement(std::move(actionMenu));

		return std::move(ui);
	}

	// Constructor

	GameScene::GameScene(Game& game) : Scene(game)
	{
		m_UI = createGameUI();
	}

	// Getters

	std::vector<WorldObject> GameScene::itemsToCentre()
	{
		// Centre based on the board and hand
		return { *m_board, *m_hand };
	}

	// Public functions

	void GameScene::handleEvents(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event)
	{
		m_UI->handleEvents(mousePosView, event);
	}

	void GameScene::update()
	{

	}

	void GameScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*m_UI);
	}
}