#pragma once

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace Rummage
{
	class ResourceManager
	{
	private:
		const static std::vector<std::string> kAllowedExtensions;
		const static std::string kInvalidPath;

		static std::map<std::string, sf::Texture*> m_textureMap;
	public:
		const static std::string kAtlasPath;

		static sf::Texture* getTexture(const std::string& filePath);
		static void preLoadTextures();
		static void clearTextures();
	};
}