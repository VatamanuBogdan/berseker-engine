#pragma once
#include <spdlog/spdlog.h>


class DefaultLogger;

class Logger {
public:
	enum Level {
		DEBUG		= 0,
		INFO,
		WARNING,
		ERROR
	};

	virtual void LogDebug(const char* message) = 0;
	virtual void LogInfo(const char* message) = 0;
	virtual void LogWarning(const char* message) = 0;
	virtual void LogError(const char* message) = 0;

	virtual void SetLevel(Level level) = 0;

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
		logger->SetLevel(level);
	}

private:
	static std::unique_ptr<Logger> logger;
};

class DefaultLogger final : public Logger {
public:
	void LogDebug(const char* message) override;
	void LogInfo(const char* message) override;
	void LogWarning(const char* message) override;
	void LogError(const char* message) override;

	void SetLevel(Level level) override;
};


