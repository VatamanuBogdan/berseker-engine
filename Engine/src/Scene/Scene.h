#pragma once
#include "Rendering/Camera.hpp"
#include "ECS/ECS.h"


#include <vector>

class Application;

class Scene {
public:
	explicit Scene(Application *application)
		: application(application) {
	}

	virtual void Init() = 0;
	virtual void Deinit() = 0;

	virtual void OnPreUpdate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnPostUpdate() = 0;

	virtual void OnPreRendering() = 0;
	virtual void OnPostRendering() = 0;

public:
	std::vector<ECS::Entity>& GetEntities() { return entities; }
	ECS::Registry& GetRegistry() { return registry; }
	Camera& GetCamera() { return *camera; }

protected:
	Application				*application;
	ECS::Registry			 registry;
	std::vector<ECS::Entity>	 entities;
	std::shared_ptr<Camera>		 camera;
};