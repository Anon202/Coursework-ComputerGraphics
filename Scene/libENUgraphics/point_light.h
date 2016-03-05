#pragma once

#include "..\\myScene\\Light.h"

namespace graphics_framework
{
	/*
	An object representing point light data
	*/
	class point_light : public Light
	{
	private:
		// The constant factor of the attenuation
		float _constant;
		// The linear factor of the attenuation
		float _linear;
		// The quadratic factor of the attenuation
		float _quadratic;
	public:
		// Creates a point light with a default colour
		point_light() : Light()
		{
			_constant = 0.5f;
			_linear = 0.2f;
			_quadratic = 0.01f;
		}
		// Creates a point light with provided properties
		point_light(const glm::vec4 &colour, const glm::vec3 &pos, float constant, float linear, float quadratic) :
			_constant(constant),
			_linear(linear),
			_quadratic(quadratic)
		{
			set_light_colour(colour);
			set_position(pos);
		}
		// Default copy constructor and assignment operator
		point_light(const point_light &other) = default;
		point_light& operator=(const point_light &rhs) = default;
		
		// Gets the constant factor of the light attenuation
		float get_constant_attenuation() const { return _constant; }
		// Sets the constant factor of the light attenuation
		void set_constant_attenuation(float value) { _constant = value; }
		// Gets the linear factor of the light attenuation
		float get_linear_attenuation() const { return _linear; }
		// Sets the linear factor of the light attenuation
		void set_linear_attenuation(float value) { _linear = value; }
		// Gets the quadratic factor of the light attenuation
		float get_quadratic_attenuation() const { return _quadratic; }
		// Sets the quadratic factor of the light attenuation
		void set_quadratic_attenuation(float value) { _quadratic = value; }
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
	};
}