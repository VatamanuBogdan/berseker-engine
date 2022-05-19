#pragma once
#include <Application.h>


class EditorApplication : public Application {
public:
	void UpdateStage(double deltaTime) override;
	void RenderStage() override;
};

