#include "game.hpp"

#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <memory>

#include "board.hpp"
#include "utils.hpp"
#include "resources.hpp"

#include "ui/ui.hpp"
#include "ui/button.hpp"
#include "ui/input_field.hpp"
#include "ui/image.hpp"

#include "scenes/menu_scene.hpp"
#include "scenes/game_scene.hpp"

namespace Rummage
{
	// Private functions

	void Game::initWindow()
	{
		m_videoMode = sf::VideoMode({ kInitWindowWidth, kInitWindowHeight });
		m_window = std::make_unique<sf::RenderWindow>(m_videoMode, "Rummage", kWindowStyle);

		// Set the window to centre on the view contents
		resizeView(kInitWindowWidth, kInitWindowHeight);

		// Set FPS
		m_window->setVerticalSyncEnabled(true); // VSync
		//this->window.setFramerateLimit(60);
	}

	sf::Vector2f Game::getGameWorldSize()
	{
		if (auto* scene = m_sceneManager.getCurrentScene())
		{
			return WorldObject::getBoundingBoxSize(scene->itemsToCentre());
		}

		return sf::Vector2f();
	}

	sf::Vector2f Game::getGameWorldCentre()
	{
		if (auto* scene = m_sceneManager.getCurrentScene())
		{
			return WorldObject::getBoundingBoxCentre(scene->itemsToCentre());
		}

		return sf::Vector2f();
	}

	void Game::createDeck()
	{
		m_deck.clear();

		// 2 * 4 suits * 12 ranks + 2 jokers = 98 tiles
		for (size_t i = 0; i < 2; i++)
		{
			// Suit
			for (int s = 1; s < SUIT_MAX; s++)
			{
				// Rank
				for (int r = 1; r < RANK_MAX; r++)
				{
					m_deck.push_back(Tile::createTile(static_cast<Suit>(s), static_cast<Rank>(r)));
				}
			}

			// Joker
			//m_deck.push_back(Tile::createTile(SUIT_NONE, RANK_NONE));
		}

		// Shuffle the elements
		for (size_t i = m_deck.size() - 1; i > 0; i--)
		{
			size_t j = rand() % i;
			std::iter_swap(m_deck.begin() + i, m_deck.begin() + j);
		}
	}

	void Game::pollEvents(const sf::Vector2f& mousePosView)
	{
		while (const std::optional event = m_window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				closeWindow();
			}

			if (const auto* resized = event->getIf<sf::Event::Resized>())
			{
				resizeView(resized->size.x, resized->size.y);
			}

			// Poll current scene
			if (auto* scene = m_sceneManager.getCurrentScene())
			{
				scene->handleEvents(mousePosView, event);
			}
		}
	}

	void Game::resizeView(int windowWidth, int windowHeight)
	{
		// Make view fit the game board while keeping the board's aspect and an integer scaling
		if (getGameWorldSize().x != 0 && getGameWorldSize().y != 0)
		{
			// Calculate view size based on integer scale
			int scale = std::max(1.f, std::min(windowWidth / getGameWorldSize().x, windowHeight / getGameWorldSize().y));

			float viewWidth = std::floor(static_cast<float>(windowWidth) / scale);
			float viewHeight = std::floor(static_cast<float>(windowHeight) / scale);

			m_view.setSize({ viewWidth, viewHeight });
			m_view.setCenter({ std::floor(getGameWorldCentre().x), std::floor(getGameWorldCentre().y) });

			m_window->setView(m_view);
		}
	}

	void Game::checkScenes()
	{
		// Check to see if a scene needs to be popped
		if (m_sceneManager.popRequested())
		{
			m_sceneManager.popScene();
			resizeView(m_window->getSize().x, m_window->getSize().y);
		}
	}

	// Constructor and Destructor

	Game::Game()
	{ 
		srand(time(NULL));

		// Preload resources
		if (sf::Shader::isAvailable())
		{
			ResourceManager::preLoadShaders();
		}

		ResourceManager::preLoadTextures();

		// Initially load menu scene
		m_sceneManager.pushScene(std::make_unique<MenuScene>(*this));

		initWindow();
	}

	// Getters

	bool Game::isRunning() const
	{
		return m_window->isOpen();
	}

	// Public functions

	void Game::startGame()
	{
		// Create deck
		createDeck();

		// Load game scene
		m_sceneManager.changeScene(std::make_unique<GameScene>(*this));

		m_gameStarted = true;
	}

	void Game::closeWindow()
	{
		m_window->close();
	}

	void Game::update()
	{
		sf::Vector2f mousePosView = getMousePosView();

		pollEvents(mousePosView);

		// Update current scene
		if (auto* scene = m_sceneManager.getCurrentScene())
		{
			scene->update(mousePosView);
		}

		checkScenes();
	}

	void Game::render()
	{
		m_window->clear(sf::Color(92, 214, 92));

		// Draw current scene
		if (auto* scene = m_sceneManager.getCurrentScene())
		{
			m_window->draw(*scene);
		}

		m_window->display();
	}
}