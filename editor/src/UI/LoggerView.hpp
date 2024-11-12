#include "Views.h"

#include <imgui.h>

#include <string>
#include <chrono>

class LoggerView : public View {
public:
	static constexpr size_t DEFAULT_BUFFER_MAX_SIZE = 1 << 16;

	explicit LoggerView(size_t maxBufferSize = DEFAULT_BUFFER_MAX_SIZE)
		: maxBufferSize(maxBufferSize) {
		auto clock = std::chrono::system_clock::now();
		auto tt = std::chrono::system_clock::to_time_t(clock);
		time = std::ctime(&tt);
		if (time.back() == '\n')
			time.pop_back();
	}

	void Draw() override {
		const char *fmt = "[%s] [%s] %s";

		ImGui::Text(fmt, time.c_str(), "info", "OpenGL version 4.6 (Core Profile) Mesa 22.1.2");
		ImGui::Text(fmt, time.c_str(), "info", "OpenGL shading language 4.60");
		ImGui::Text(fmt, time.c_str(), "info", "Texture slots 32");
	}

private:
	std::string time;
	std::string buffer;
	size_t	maxBufferSize;
};