#pragma once
#include "UI/UIRenderer.h"
#include "Window/GLFW/GLFWWindow.h"


class UIRendererImpl_GL_GLFW final : public UIRenderer {
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
	void Init() final;
	void Deinit() final;

private:
	void PreRendering() final;
	void PostRendering() final;

private:
	GLFWwindow 	*window;
	GLSLVersion  glslVersion;
};
