#include "EditorApplication.h"
#include "Core/Logger.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	try {
		EditorApplication application;

		application.Init();
		application.StartRunning();
		application.Deinit();
	} catch (std::exception &ex) {
		Logger::Log<Logger::ERROR>("Exception received on application running {}", ex.what());
	}
	return 0;
}