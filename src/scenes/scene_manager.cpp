#include "scene_manager.hpp"

#include <stack>
#include <memory>

namespace Rummage
{
	void SceneManager::pushScene(std::unique_ptr<Scene> scene)
	{
		m_scenes.push(std::move(scene));
	}

	void SceneManager::requestPop()
	{
		// Executions must be finished before a scene can be popped
		m_popRequested = true;
	}

	void SceneManager::popScene()
	{
		if (!m_scenes.empty())
		{
			m_scenes.pop();
			m_popRequested = false;
		}
	}

	void SceneManager::changeScene(std::unique_ptr<Scene> scene)
	{
		requestPop();
		pushScene(std::move(scene));
	}

	Scene* SceneManager::getCurrentScene()
	{
		if (m_scenes.empty()) return nullptr;

		return m_scenes.front().get();
	}
}