#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include "..\\myScene\\Light.h"

namespace graphics_framework
{
	// An object representing spot light data
	class spot_light : public Light
	{
	private:
		// The constant factor of the attenuation
		float _constant;
		// The linear factor of the attenuation
		float _linear;
		// The quadratic factor of the attenuation
		float _quadratic;
		// The power of the spot light
		float _power;
	public:
		// Creates a spot light with a default colour
		spot_light() : Light() 
		{
			_constant = 0.5f;
			_linear = 0.2f;
			_quadratic = 0.01f;
			_power = 10.0f;
		}
		// Creates a spot light with the provided properties
		spot_light(const glm::vec4 &colour, const glm::vec3 &position, const glm::vec3 &direction, float constant, float linear, float quadratic, float power) :
			_constant(constant),
			_linear(linear),
			_quadratic(quadratic),
			_power(power)
		{
			set_light_colour(colour);
			set_position(position);
			set_direction(direction);

		}
		// Default copy constructor and assignment operator
		spot_light(const spot_light &other) = default;
		spot_light& operator=(const spot_light &other) = default;

		
		// Gets the constant factor of the light attenuation
		float get_constant_attenuation() const { return _constant; }
		// Sets the constant factor of the light attenuation
		float set_constant_attenuation(float value) { _constant = value; }
		// Gets the linear factor of the light attenuation
		float get_linear_attenuation() const { return _linear; }
		// Sets the linear factor of the light attenuation
		void set_linear_attenuation(float value) { _linear = value; }
		// Gets the quadratic factor of the light attenuation
		float get_quadratic_attenuation() const { return _quadratic; }
		// Sets the quadratic factor of the light attenuation
		void set_quadratic_attenuation(float value) { _quadratic = value; }
		// Gets the power of the spot light
		float get_power() const { return _power; }
		// Sets the power of the spot light
		void set_power(float value) { _power = value; }
		// Sets the range of the point light
		void set_range(float range)
		{
			_linear = 2.0f / range;
			_quadratic = 1.0f / (powf(range, 2.0f));
		}
		// Moves the light by the given vector
		void move(const glm::vec3 &translation)
		{
			_position += translation;
		}
		// Rotates the light
		void rotate(const glm::quat &rotation)
		{
			// Calculate new orientation
			auto rot = glm::mat3_cast(rotation);
			_direction = rot * _direction;
		}
		// Rotates the light
		void rotate(const glm::vec3 &rotation)
		{
			rotate(glm::quat(rotation));
		}
	};
}