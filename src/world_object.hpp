#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "utils.hpp"

namespace Rummage
{
	struct Padding
	{
		int l;
		int r;
		int t;
		int b;

		Padding(int _l, int _r, int _t, int _b) : l(_l), r(_r), t(_t), b(_b) {}
	};

	// World objects have:
	// - a size (sf::Vector2f)
	// - a position (sf::Vector2f)
	// These are (0, 0) by default and must be set.
	// This is needed for bounds checks.
	class WorldObject
	{
	protected:
		sf::Vector2f m_pos = { 0, 0 };  // Position of the board in pixels
		sf::Vector2f m_size = { 0, 0 }; // Size of board in pixels

		Padding m_padding = { 0, 0, 0, 0 };   // Padding in pixels
	public:
		WorldObject(sf::Vector2f pos = {0, 0}, Padding padding = {0, 0, 0, 0}) : m_pos(pos), m_padding(padding) {}
		virtual ~WorldObject() = default;

		// Getters

		sf::Vector2f getPos() const { return m_pos; }
		sf::Vector2f getCentrePos() const { return m_pos + m_size / 2.f; }
		sf::Vector2f getSize() const { return m_size; }

		// Setters

		void setPadding(const Padding padding) { m_padding = padding; }
		virtual void setPos(const sf::Vector2f& newPos) { m_pos = newPos; } // Override to move the sprite

		// Static utility methods

		// Get the bounding box size vector of a list of WorldObjects.
		static sf::Vector2f getBoundingBoxSize(std::vector<WorldObject> objs)
		{
			if (objs.size() == 0) return { 0, 0 };

			float minX = 0, minY = 0, maxX = 0, maxY = 0;

			for (WorldObject& obj : objs)
			{
				if (obj.m_pos.x < minX) minX = obj.m_pos.x;
				if (obj.m_pos.y < minY) minY = obj.m_pos.y;

				if (obj.m_pos.x + obj.m_size.x > maxX) maxX = obj.m_pos.x + obj.m_size.x;
				if (obj.m_pos.y + obj.m_size.y > maxY) maxY = obj.m_pos.y + obj.m_size.y;
			}

			return { maxX - minX, maxY - minY };
		}

		// Get the bounding box position vector of the centre of a list of WorldObjects
		static sf::Vector2f getBoundingBoxCentre(std::vector<WorldObject> objs)
		{
			if (objs.size() == 0) return { 0, 0 };

			float minX = 0, minY = 0, maxX = 0, maxY = 0;

			for (WorldObject& obj : objs)
			{
				if (obj.m_pos.x < minX) minX = obj.m_pos.x;
				if (obj.m_pos.y < minY) minY = obj.m_pos.y;

				if (obj.m_pos.x + obj.m_size.x > maxX) maxX = obj.m_pos.x + obj.m_size.x;
				if (obj.m_pos.y + obj.m_size.y > maxY) maxY = obj.m_pos.y + obj.m_size.y;
			}

			return { (maxX - minX) / 2 + minX, (maxY - minY) / 2 + minY, };
		}
	};
}