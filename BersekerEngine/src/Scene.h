#pragma once
#include "Rendering//Camera.hpp"


class Scene {
public:
	virtual void Init() = 0;
	virtual void Deinit() = 0;

	virtual void OnPreUpdate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnPostUpdate() = 0;

	virtual void OnPreRendering() = 0;
	virtual void OnPostRendering() = 0;
};