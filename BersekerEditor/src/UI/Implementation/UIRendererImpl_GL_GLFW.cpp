#include "UIRendererImpl_GL_GLFW.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <cstdio>


UIRendererImpl_GL_GLFW::UIRendererImpl_GL_GLFW(GLFWwindow *window, GLSLVersion glslVersion)
	: window(window), glslVersion(glslVersion){
}

void UIRendererImpl_GL_GLFW::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	constexpr unsigned versionBufLength = 256;
	char versionBuf[versionBufLength];
	std::snprintf(versionBuf, versionBufLength, "#version %d%d0", glslVersion.Major, glslVersion.Minor);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(versionBuf);
	ImGui::StyleColorsLight();
}

void UIRendererImpl_GL_GLFW::PreUIRendering() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UIRendererImpl_GL_GLFW::PostUIRendering() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIRendererImpl_GL_GLFW::Deinit() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}