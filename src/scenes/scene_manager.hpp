#pragma once

#include <memory>
#include <stack>

#include "scene.hpp"

namespace Rummage
{
	class SceneManager
	{
	private:
		std::stack<std::unique_ptr<Scene>> m_scenes;
	public:
		void pushScene(std::unique_ptr<Scene> scene);
		void popScene();
		void changeScene(std::unique_ptr<Scene> scene);

		Scene* getCurrentScene();
	};
}