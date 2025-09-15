#include "resources.hpp"

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace Rummage
{
	// Initialise data members

	const std::vector<std::string> ResourceManager::kAllowedExtensions = { "png", "jpg", "jpeg" };
	const std::string ResourceManager::kInvalidPath = IMAGES_PATH "Invalid.png";
	const std::string ResourceManager::kAtlasPath = IMAGES_PATH "RummageTextureAtlas.png";

	std::map<std::string, sf::Texture*> ResourceManager::m_textureMap;
	std::map<std::string, sf::Shader*> ResourceManager::m_shaderMap;

	// Public functions

	// Textures functions

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

	// Shader functions

	sf::Shader* ResourceManager::getShader(const std::string& filePath, sf::Shader::Type type)
	{
		if (sf::Shader::isAvailable())
		{
			// Search through the map for the entry
			for (auto& item : m_shaderMap)
			{
				if (item.first == filePath)
				{
					return item.second;
				}
			}

			// Create new map item

			sf::Shader* shader = new sf::Shader();
			if (shader->loadFromFile(filePath, type)) {
				m_shaderMap[filePath] = shader;
				return m_shaderMap[filePath];
			}
		}

		return nullptr;
	}

	void ResourceManager::preLoadShaders()
	{
		// Needs:
		// setUniform("texture", sprite.getTexture());
		// setUniform("offset", 1.0f / sprite.getTexture().getSize().x);
		// setUniform("colour", sf::Glsl::Vec4 {1.0, 1.0, 1.0, 1.0})
		getShader(SHADERS_PATH "outline.shader", sf::Shader::Type::Fragment);

		// Needs:
		// setUniform("texture", sprite.getTexture());
		// setUniform("colour", sf::Glsl::Vec4 {1.0, 1.0, 1.0, 1.0})
		getShader(SHADERS_PATH "highlight.shader", sf::Shader::Type::Fragment);
	}

	void ResourceManager::clearShaders()
	{
		// Delete all pointer

		for (auto& item : m_shaderMap)
		{
			delete item.second;
		}

		m_textureMap.clear();
	}
}