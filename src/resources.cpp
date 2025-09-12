#include "resources.hpp"

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace Rummage
{
	// Initialise data members

	const std::vector<std::string> ResourceManager::kAllowedExtensions = { "png", "jpg", "jpeg" };
	const std::string ResourceManager::kInvalidPath = "../../../../resources/images/Invalid.png";
	const std::string ResourceManager::kAtlasPath = "../../../../resources/images/RummageTextureAtlas.png";

	std::map<std::string, sf::Texture*> ResourceManager::m_textureMap;

	// Public functions

	sf::Texture* ResourceManager::getTexture(const std::string& filePath)
	{
		// Search through the map for the entry
		for (auto& item : m_textureMap)
		{
			if (item.first == filePath)
			{
				return item.second;
			}
		}

		// Create new map item

		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(filePath)) {
			// If invalid, load the invalid texture path
			texture->loadFromFile(kInvalidPath);
		}

		m_textureMap[filePath] = texture;
		return m_textureMap[filePath];
	}

	void ResourceManager::preLoadTextures()
	{
		getTexture(kAtlasPath);
	}

	void ResourceManager::clearTextures()
	{
		// Delete all pointer

		for (auto& item : m_textureMap)
		{
			delete item.second;
		}

		m_textureMap.clear();
	}
}