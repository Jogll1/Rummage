#include "scene_manager.hpp"

#include <stack>
#include <memory>

namespace Rummage
{
	void SceneManager::pushScene(std::unique_ptr<Scene> scene)
	{
		m_scenes.push(std::move(scene));
	}

	void SceneManager::popScene()
	{
		if (!m_scenes.empty())
		{
			m_scenes.pop();
		}
	}

	void SceneManager::changeScene(std::unique_ptr<Scene> scene)
	{
		popScene();
		pushScene(std::move(scene));
	}

	Scene* SceneManager::getCurrentScene()
	{
		if (m_scenes.empty()) return nullptr;

		return m_scenes.top().get();
	}
}