#pragma once
#include <spdlog/spdlog.h>

class Logger {
public:
	enum Level {
		DEBUG		= 0,
		INFO,
		WARNING,
		ERROR
	};

public:
	template <Level level, typename... Args>
	static void Log(const char *format, Args... args) {
		if constexpr(level == DEBUG) {
			spdlog::debug(format, args...);
		} else if constexpr(level == INFO) {
			spdlog::info(format, args...);
		} else if constexpr(level == WARNING) {
			spdlog::warn(format, args...);
		} else if constexpr(level == ERROR) {
			spdlog::error(format, args...);
		}
	}

	static void SetLoggerLevel(Level level) {
		spdlog::level::level_enum spdlogLevel;
		switch (level) {
			case DEBUG:
				spdlogLevel = spdlog::level::debug;
				break;
			case INFO:
				spdlogLevel = spdlog::level::info;
				break;
			case WARNING:
				spdlogLevel = spdlog::level::warn;
				break;
			case ERROR:
				spdlogLevel = spdlog::level::err;
				break;
		}
		spdlog::set_level(spdlogLevel);
	}
};


