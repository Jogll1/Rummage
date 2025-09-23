#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

namespace Rummage
{
	static void printVector(sf::Vector2f vector, bool newline)
	{
		std::cout << "(" << vector.x << ", " << vector.y << ")";

		if (newline)
		{
			std::cout << "\n";
		}
	}

	static inline sf::Vector2f roundVector(sf::Vector2f vector)
	{
		return sf::Vector2f(std::floor(vector.x), std::floor(vector.y));
	}
}