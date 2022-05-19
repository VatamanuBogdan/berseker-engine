#pragma once
#include "UI/UIRendererBackend.h"
#include "Window/GLFW/GLFWWindow.h"


class UIRendererImpl_GL_GLFW : public UIRendererBackend {
public:
	struct GLSLVersion {
		const unsigned Major;
		const unsigned Minor;

		GLSLVersion(unsigned major, unsigned minor)
			: Major(major), Minor(minor) {
		}
	};

public:
	UIRendererImpl_GL_GLFW(GLFWwindow *window, GLSLVersion glslVersion);

public:
	void Init() override;
	void Deinit() override;

protected:
	void PreUIRendering() final;
	void PostUIRendering() final;

private:
	GLFWwindow 	*window;
	GLSLVersion  glslVersion;
};
