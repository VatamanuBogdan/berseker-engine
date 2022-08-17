#pragma once
#include "Scene/Camera.hpp"
#include "Ecs/Registry.h"


#include <vector>
#include <memory>

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
	std::vector<Ecs::Entity>& GetEntities() { return entities; }
	Ecs::Registry& GetRegistry() { return registry; }
	Camera& GetCamera() { return *camera; }

protected:
	Application				*application;
	Ecs::Registry			 registry;
	std::vector<Ecs::Entity>	 entities;
	std::shared_ptr<Camera>		 camera;
};