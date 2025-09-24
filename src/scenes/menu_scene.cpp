#include "menu_scene.hpp"

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

	void MenuScene::openJoinMenu()
	{
		for (auto& obj : m_UI->getElements())
		{
			obj->visible = obj->tag != "main";
		}
	}

	void MenuScene::closeJoinMenu()
	{
		for (auto& obj : m_UI->getElements())
		{
			obj->visible = obj->tag != "join";
		}
	}

	std::unique_ptr<UI> MenuScene::createMenuUI()
	{
		std::unique_ptr<UI> ui = std::make_unique<UI>();

		std::unique_ptr<UIImage> logo = std::make_unique<UIImage>(sf::IntRect({ 286, 22 }, { 74, 74 }), sf::Vector2f(0, 0), Padding(10, 10, 10, 4));
		sf::Vector2f logoSize = logo->getSize();

		// === Main ===

		std::unique_ptr<UIVGroup> mainMenu = std::make_unique<UIVGroup>(2.f, sf::Vector2f(0, 0), Padding(0, 0, 4, 4));
		mainMenu->tag = "main";

		std::unique_ptr<Button> hostButton = std::make_unique<Button>("HOST", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		hostButton->setCallback([this]() { this->m_game.startGame(); });
		std::unique_ptr<Button> joinButton = std::make_unique<Button>("JOIN", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		joinButton->setCallback([this]() { this->openJoinMenu(); });
		std::unique_ptr<Button> quitButton = std::make_unique<Button>("QUIT", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		quitButton->setCallback([this]() { this->m_game.closeWindow(); });

		mainMenu->addElement(std::move(hostButton));
		mainMenu->addElement(std::move(joinButton));
		mainMenu->addElement(std::move(quitButton));
		mainMenu->setPos({ logoSize.x / 2.f - mainMenu->getSize().x / 2.f, logoSize.y });

		// === Join === 

		std::unique_ptr<UIVGroup> joinMenu = std::make_unique<UIVGroup>(2.f, sf::Vector2f(0, 0), Padding(0, 0, 4, 4));
		joinMenu->tag = "join";
		joinMenu->visible = false;

		std::unique_ptr<InputField> roomInput = std::make_unique<InputField>(sf::IntRect({ 224, 3 }, { 36, 16 }));
		std::unique_ptr<Button> playButton = std::make_unique<Button>("PLAY", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		playButton->setCallback([this]() { this->m_game.startGame(); });
		std::unique_ptr<Button> backButton = std::make_unique<Button>("BACK", sf::IntRect({ 136, 3 }, { 36, 16 }), sf::IntRect({ 180, 3 }, { 36, 16 }));
		backButton->setCallback([this]() { this->closeJoinMenu(); });

		joinMenu->addElement(std::move(roomInput));
		joinMenu->addElement(std::move(playButton));
		joinMenu->addElement(std::move(backButton));
		joinMenu->setPos({ logoSize.x / 2.f - joinMenu->getSize().x / 2.f, logoSize.y });

		// === Adding ===

		ui->addElement(std::move(logo));
		ui->addElement(std::move(mainMenu));
		ui->addElement(std::move(joinMenu));

		return std::move(ui);
	}

	// Constructor

	MenuScene::MenuScene(Game& game) : Scene(game)
	{
		m_UI = createMenuUI();
	}

	// Getters

	std::vector<WorldObject> MenuScene::itemsToCentre()
	{
		// Centre based on UI elements of the scene
		std::vector<WorldObject> objs;
		for (const auto& obj : m_UI->getElements())
		{
			objs.push_back(*obj);
		}

		return objs;
	}

	// Public functions

	void MenuScene::handleEvents(const sf::Vector2f& mousePosView, const std::optional<sf::Event> event)
	{
		m_UI->handleEvents(mousePosView, event);
	}

	void MenuScene::update(const sf::Vector2f& mousePosView)
	{

	}

	void MenuScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*m_UI);
	}
}