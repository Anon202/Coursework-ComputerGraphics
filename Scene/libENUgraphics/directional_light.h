#pragma once

#include "..\\myScene\\Light.h"

namespace graphics_framework
{
	/*
	Object that describes a directional light
	*/
	class directional_light : public Light
	{
	private:
		// Ambient intensity of the light
		glm::vec4 _ambient;
	public:
		// Creates a directional light with some default colour values
		directional_light() : Light()
		{
			_ambient = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
		}
		
		// Creates a directional light with provided values
		directional_light(
			const glm::vec4 &ambient_intensity,
			const glm::vec4 &light_colour,
			const glm::vec3 &direction) :
			_ambient(ambient_intensity)
		{
			set_light_colour(light_colour);
			set_direction(direction);
		}
		// Default copy constructor and assignment operator
		directional_light(const directional_light &other) = default;
		directional_light& operator=(const directional_light &rhs) = default;
		// Gets the ambient intensity of the light
		glm::vec4 get_ambient_intensity() const { return _ambient; }
		// Sets the ambient intensity of the light
		void set_ambient_intensity(const glm::vec4 &value) { _ambient = value; }
		// Rotates the light using the given Euler angles
		void rotate(const glm::vec3 &rotation);
		// Rotates the light using the given quaternion
		void rotate(const glm::quat &rotation);
	};
}