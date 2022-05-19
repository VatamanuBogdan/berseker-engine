#include "Application.h"
#include "Scenes/BVolumes/Scene.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	try {
		Application application;

		std::shared_ptr<Scene> scene = std::make_shared<BVolumesTesting::Scene>(&application);
		application.Init(scene);
		application.StartRunning();
		application.Deinit();
	} catch (std::exception &ex) {
		std::cerr << ex.what();
	}
	return 0;
}