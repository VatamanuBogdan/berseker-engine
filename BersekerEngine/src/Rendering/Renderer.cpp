#include "Rendering/Renderer.h"

PrimitivesRenderer				Renderer::primitivesRender;
std::shared_ptr<Camera> 			Renderer::camera;
std::vector<Renderer::RenderingEntity>	Renderer::renderingQueue;

void Renderer::Init() {
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	primitivesRender.Init();
}

void Renderer::Deinit() {
	primitivesRender.Deinit();
}

void Renderer::SetCamera(std::shared_ptr<Camera> &camera) {
	Renderer::camera = camera;
	primitivesRender.AttachCamera(camera);
}

void Renderer::SubmitForRendering(const VertexArray 			*vao,
					    const IndexBuffer			*ibo,
					    ShaderProgram 			*shader,
					    const glm::mat4 			&model) {
	renderingQueue.emplace_back(vao, ibo, shader, model);
}

void Renderer::RenderCuber(const CubeProps &props) {
	primitivesRender.RenderCube(props);
}

void Renderer::RenderClearColor(float red, float green, float blue, float alpha) {
	glClearColor(red, green, blue, alpha);
}

void Renderer::Render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto &renderingObject : renderingQueue) {
		auto shader = renderingObject.shaderProgram;
		shader->Bind();
		shader->SetUniform("Projection", Renderer::camera->GetProjection());
		shader->SetUniform("Model", renderingObject.model);
		shader->SetUniform("View", Renderer::camera->GetView());

		renderingObject.vao->Bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		OpenGL::DrawElementsWith(*renderingObject.ibo);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	renderingQueue.clear();
}
