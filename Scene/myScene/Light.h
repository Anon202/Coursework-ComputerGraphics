/*
*	Light.h - Base class abstraction for light types.
*	created for ease of storage (polymorphism)
*/

#pragma once

#include "stdafx.h"

// base light class for light types.
namespace graphics_framework
{

	class Light
	{
	protected:
		glm::vec4 _colour = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		glm::vec3 _direction = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);

	public:
		// sets default light
		Light() = default;
		virtual ~Light() { }

		// getters and setters
		glm::vec4 get_light_colour() const { return _colour; }
		void set_light_colour(const glm::vec4 &value) { _colour = value; }

		glm::vec3 get_direction() const { return _direction; }
		void set_direction(const glm::vec3 &value) { _direction = value; }

		glm::vec3 get_position() const { return _position; }
		void set_position(const glm::vec3 &value) { _position = value; }

	};

}