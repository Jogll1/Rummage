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
}