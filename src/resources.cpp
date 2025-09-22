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

	std::map<std::string, std::shared_ptr<sf::Texture>> ResourceManager::m_textureMap;
	std::map<std::string, std::shared_ptr<sf::Shader>> ResourceManager::m_shaderMap;
	std::map<std::string, std::shared_ptr<sf::Font>> ResourceManager::m_fontMap;

	// Public functions

	// Textures functions

	std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& filePath)
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

		std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
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

	// Shader functions

	std::shared_ptr<sf::Shader> ResourceManager::getShader(const std::string& filePath, sf::Shader::Type type)
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

			std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
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
		std::shared_ptr<sf::Shader> outlineShader = getShader(SHADERS_PATH "outline.shader", sf::Shader::Type::Fragment);
		std::shared_ptr<sf::Texture> textureAtlas = getTexture(ResourceManager::kAtlasPath);

		// Set shader uniforms
		outlineShader->setUniform("texture", *textureAtlas);
		outlineShader->setUniform("offsetX", 1.0f / (*textureAtlas).getSize().x);
		outlineShader->setUniform("offsetY", 1.0f / (*textureAtlas).getSize().y);
		outlineShader->setUniform("colour", sf::Glsl::Vec4{ 1.0, 1.0, 1.0, 1.0 });

		// Needs:
		// setUniform("texture", sprite.getTexture());
		// setUniform("colour", sf::Glsl::Vec4 {1.0, 1.0, 1.0, 1.0})
		getShader(SHADERS_PATH "highlight.shader", sf::Shader::Type::Fragment);
	}

	std::shared_ptr<sf::Font> ResourceManager::getFont(const std::string& filePath)
	{
		// Search through the map for the entry
		for (auto& item : m_fontMap)
		{
			if (item.first == filePath)
			{
				return item.second;
			}
		}

		// Create new map item

		std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();
		if (!font->openFromFile(filePath)) {
			// If font invalid, crash (CHANGE)
			exit(0);
		}

		m_fontMap[filePath] = font;
		return m_fontMap[filePath];
	}

	void ResourceManager::preLoadFonts()
	{
		getFont(RESOURCES_PATH "8bit-ascii.ttf");
	}
}