#include "Application.h"
#include "Scenes/BVolumesTesting/Scene.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	try {
		std::shared_ptr<Scene> scene = std::make_shared<BVolumesTesting::Scene>();
		Application::Init(scene);
		Application::StartRunning();
		Application::Deinit();
	} catch (std::exception &ex) {
		std::cerr << ex.what();
	}
	return 0;
}