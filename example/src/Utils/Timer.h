#pragma once
#include <chrono>

class Timer {
public:
	void Start() {
		startTime = std::chrono::high_resolution_clock::now();
	}

	long Stop() {
		auto endTime = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};