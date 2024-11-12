#pragma once
#include <random>

class Random {
public:
	Random(float start, float end)
		: mt(std::random_device()()), distr(start, end) {
	}

	float Generate() {
		return distr(mt);
	}

private:
	std::mt19937 mt;
	std::uniform_real_distribution<float> distr;
};