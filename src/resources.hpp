#pragma once

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#define IMAGES_PATH "../../../../resources/images/"
#define SHADERS_PATH "../../../../resources/shaders/"

namespace Rummage
{
	class ResourceManager
	{
	private:
		const static std::vector<std::string> kAllowedExtensions;
		const static std::string kInvalidPath;

		static std::map<std::string, sf::Texture*> m_textureMap;
		static std::map<std::string, sf::Shader*> m_shaderMap;
	public:
		const static std::string kAtlasPath;

		// Texture functions

		static sf::Texture* getTexture(const std::string& filePath);
		static void preLoadTextures();
		static void clearTextures();

		// Shader functions

		static sf::Shader* getShader(const std::string& filePath, sf::Shader::Type type);
		static void preLoadShaders();
		static void clearShaders();
	};
}