#include "Application.h"
#include "TestScene/MainScene.h"

int main(int argc, char *argv[]) {
	try {
		std::shared_ptr<Scene> scene = std::make_shared<MainScene>();
		Application::Init(scene);
		Application::StartRunning();
		Application::Deinit();
	} catch (std::exception &ex) {
		std::cerr << ex.what();
	}
	return 0;
}