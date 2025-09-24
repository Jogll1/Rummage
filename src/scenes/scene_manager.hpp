#pragma once

#include <memory>
#include <queue>

#include "scene.hpp"

namespace Rummage
{
	class SceneManager
	{
	private:
		bool m_popRequested = false;

		std::queue<std::unique_ptr<Scene>> m_scenes;
	public:
		bool popRequested() const { return m_popRequested; }

		void pushScene(std::unique_ptr<Scene> scene);
		void requestPop();
		void popScene();

		void changeScene(std::unique_ptr<Scene> scene);

		Scene* getCurrentScene();
	};
}