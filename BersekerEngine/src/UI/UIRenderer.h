#pragma once
#include "Utils.h"

#include <memory>

class UIController {
public:
	virtual void OnUpdate() = 0;
	virtual void OnRendering() = 0;
};


class UIRenderer {
public:
	virtual void Init() = 0;
	virtual void Deinit() = 0;

	virtual void Update() {
		SafeNullableCall(controller, OnUpdate());
	}

	virtual void Render() {
		PreRendering();
		SafeNullableCall(controller, OnRendering());
		PostRendering();
	}

public:
	void BindController(std::shared_ptr<UIController> &controller) {
		this->controller = controller;
	}

private:
	virtual void PreRendering() = 0;
	virtual void PostRendering() = 0;

private:
	std::shared_ptr<UIController> controller = nullptr;
};