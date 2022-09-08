#include "Logger.h"


std::unique_ptr<Logger> Logger::logger = std::make_unique<DefaultLogger>();

void DefaultLogger::LogDebug(const char *message) {
	spdlog::debug(message);
}

void DefaultLogger::LogInfo(const char *message) {
	spdlog::info(message);
}

void DefaultLogger::LogWarning(const char *message) {
	spdlog::warn(message);
}

void DefaultLogger::LogError(const char *message) {
	spdlog::error(message);
}

void DefaultLogger::SetLevel(Logger::Level level) {
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
