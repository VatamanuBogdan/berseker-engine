#pragma once
#include <stdexcept>

#include <glm/glm.hpp>

struct Color {
	Color(float red, float green, float blue, float alpha = 1.0f)
		: Red(red), Green(green), Blue(blue), Alpha(alpha) {
		if (red < 0 || red > 1
		  	|| green < 0 || green > 1
		  	|| blue < 0 || blue > 1
		  	|| alpha < 0 || alpha > 1) {
			throw std::invalid_argument("Every spectrum color value must be in [0.0f..1.0f] range");
		}
	}

	explicit operator glm::vec4 () const {
		return {Red, Green, Blue, Alpha};
	}

	float Red;
	float Green;
	float Blue;
	float Alpha;
};