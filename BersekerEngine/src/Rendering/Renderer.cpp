#include "Rendering/Renderer.h"

PrimitivesRenderer				Renderer::primitivesRender;
std::shared_ptr<Camera> 			Renderer::camera;
std::vector<Renderer::RenderingEntity>	Renderer::renderingQueue;
Lateinit<ShaderProgram>				Renderer::colliderShader;

void Renderer::Init() {
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	primitivesRender.Init();
	colliderShader.Init(ShaderProgram::LoadFrom(
		  "res/shaders/Collider.vert.glsl",
		  "res/shaders/Collider.frag.glsl"
	));
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

void Renderer::RenderCube(const CubeProps &props) {
	primitivesRender.RenderCube(props);
}

void Renderer::RenderSphere(const SphereProps &props) {
	primitivesRender.RenderSphere(props);
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

void Renderer::RenderBVolume(const BVolumes::BVolume &bVolume, const Color &color) {
	PrimitiveBody body(PrimitiveBody::Type::SHAPED, color);

	switch (bVolume.TypeTag) {
		case BVolumes::BVolume::AABB: {
				auto &aabb = reinterpret_cast<const BVolumes::AABB&>(bVolume);
				Primitive3DProps props(aabb.GetPosition(), glm::vec3(0), aabb.GetWidths(), body);
				RenderCube(CubeProps(1, props));
			}
			break;
		case BVolumes::BVolume::OBB: {
				auto &obb = reinterpret_cast<const BVolumes::OBB&>(bVolume);
				Primitive3DProps props(obb.GetPosition(), obb.GetRotation(), obb.GetWidths(), body);
				RenderCube(CubeProps(1, props));
			}
			break;
		case BVolumes::BVolume::Sphere: {
				auto &sphere = reinterpret_cast<const BVolumes::Sphere&>(bVolume);
				Primitive3DProps props(sphere.GetPosition(), glm::vec3(0), glm::vec3(1), body);
				RenderSphere(SphereProps(sphere.GetRadius(), props));
			}
			break;
		default:
			break;
	}
}

void Renderer::RenderCollider(const RenderableCollider &renderableCollider, const glm::mat4 &model) {
	colliderShader->Bind();
	colliderShader->SetUniform("Projection", Renderer::camera->GetProjection());
	colliderShader->SetUniform("Model", model);
	colliderShader->SetUniform("View", Renderer::camera->GetView());

	glPointSize(4.0f);
	OpenGL::DrawArrays(renderableCollider.vao, OpenGL::POINTS, 0, renderableCollider.size - 1);
	glPointSize(1.0f);
}
