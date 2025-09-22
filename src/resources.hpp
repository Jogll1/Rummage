#pragma once

#include <map>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#define RESOURCES_PATH "../../../../resources/"
#define IMAGES_PATH "../../../../resources/images/"
#define SHADERS_PATH "../../../../resources/shaders/"

namespace Rummage
{
	class ResourceManager
	{
	private:
		const static std::vector<std::string> kAllowedExtensions;
		const static std::string kInvalidPath;

		static std::map<std::string, std::shared_ptr<sf::Texture>> m_textureMap;
		static std::map<std::string, std::shared_ptr<sf::Shader>> m_shaderMap; // <- Shaders should have a name key not file path
		static std::map<std::string, std::shared_ptr<sf::Font>> m_fontMap;
	public:
		const static std::string kAtlasPath;

		// Texture functions

		static std::shared_ptr<sf::Texture> getTexture(const std::string& filePath);
		static void preLoadTextures();

		// Shader functions

		static std::shared_ptr<sf::Shader> getShader(const std::string& filePath, sf::Shader::Type type);
		static void preLoadShaders();

		// Font functions

		static std::shared_ptr<sf::Font> getFont(const std::string& filePath);
		static void preLoadFonts();
	};
}