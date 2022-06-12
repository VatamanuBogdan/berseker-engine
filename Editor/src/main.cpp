#include "EditorApplication.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	try {
		EditorApplication application;

		application.Init();
		application.StartRunning();
		application.Deinit();
	} catch (std::exception &ex) {
		std::cerr << ex.what();
	}
	return 0;
}