#include "Rendering/Renderer.h"
#include "Rendering/LightSourceBillboard.h"
#include "Physics/ColliderLoader.h"
#include "Rendering/Utils.h"

PrimitivesRenderer				Renderer::primitivesRender;
Color							Renderer::clearColor(0.0f, 0.0f, 0.0f);

std::optional<Skybox>				Renderer::skybox;

std::vector<std::pair<LightSource, glm::vec3>>	Renderer::lightSources;

std::shared_ptr<Camera> 			Renderer::camera;
std::vector<Renderer::RenderingModel>	Renderer::modelRenderingQueue;

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

void Renderer::RenderCube(const CubeProps &props) {
	primitivesRender.RenderCube(props);
}

void Renderer::RenderSphere(const SphereProps &props) {
	primitivesRender.RenderSphere(props);
}

void Renderer::RenderClearColor(const Color &color) {
	clearColor = color;
}

void Renderer::Render() {
	glClearColor(clearColor.Red, clearColor.Green, clearColor.Blue, clearColor.Alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (skybox) {
		skybox->Draw(
			  Renderer::camera->GetProjection(),
			  glm::mat3(Renderer::camera->GetView())
		);
	}

	for (auto &lightSource : lightSources) {
		LightSourceBillboard().Draw(
			  glm::translate(glm::mat4(1), lightSource.second),
			  Renderer::camera->GetView(),
			  Renderer::camera->GetProjection()
		);
	}

	for (auto &renderingModel : modelRenderingQueue) {
		size_t materialIndex = 0;
		for (auto &mesh : renderingModel.model->GetMeshes()) {
			auto &material = renderingModel.model->GetMaterialFor(materialIndex);
			auto &shader = material.Shader;
			shader->Bind();
			shader->SetUniform("Projection", Renderer::camera->GetProjection());
			shader->SetUniform("Model", renderingModel.modelMatrix);
			shader->SetUniform("View", Renderer::camera->GetView());

			if (material.IsLighted()) {
				shader->SetUniform("u_CameraPosition", camera->GetPosition());

				shader->SetUniform("u_Material.Ambient", material.Ambient);
				shader->SetUniform("u_Material.Diffuse", material.Diffuse);
				shader->SetUniform("u_Material.Specular", material.Specular);
				shader->SetUniform("u_Material.Shininess", material.Shininess);

				shader->SetUniform("u_LightsNum", (int) lightSources.size());

				for (size_t i = 0; i < lightSources.size(); i++) {
					shader->SetUniform(("u_LightSources[" + std::to_string(i) + "].Position").c_str(), lightSources[i].second);
					shader->SetUniform(("u_LightSources[" + std::to_string(i) + "].Ambient").c_str(), lightSources[i].first.Ambient);
					shader->SetUniform(("u_LightSources[" + std::to_string(i) + "].Diffuse").c_str(), lightSources[i].first.Diffuse);
					shader->SetUniform(("u_LightSources[" + std::to_string(i) + "].Specular").c_str(), lightSources[i].first.Specular);
				}


			}

			mesh.GetVertexArray().Bind();
			OpenGL::DrawElementsWith(mesh.GetIndexBuffer());
			materialIndex++;
		}

	}
	modelRenderingQueue.clear();
}


static Color colors[] = {
	  Color::FromShortRange(66, 135, 245),
	  Color::FromShortRange(66, 245, 90),
	  Color::FromShortRange(187, 59, 237),
	  Color::FromShortRange(255, 207, 33),
	  Color::FromShortRange(33, 255, 200),
	  Color::FromShortRange(32, 227, 48),
	  Color::FromShortRange(227, 32, 172),
	  Color::FromShortRange(227, 100, 32),
	  Color::FromShortRange(255, 255, 255),
};

void Renderer::RenderCollider(const RenderableCollider &renderableCollider, const Transform &transform, const std::set<size_t> &collidedComponents) {
	static std::shared_ptr<ShaderProgram> colliderShader = nullptr;
	if (!colliderShader) {
		colliderShader = ShaderRegistry::Get().GetShader(ShaderResource::Collider);
	}

	colliderShader->Bind();
	colliderShader->SetUniform("Projection", Renderer::camera->GetProjection());
	colliderShader->SetUniform("Model", transform.ComputeTransformMatrix());
	colliderShader->SetUniform("View", Renderer::camera->GetView());

	glPointSize(20 / glm::length(Renderer::camera->GetPosition() - transform.Position));
	for (size_t i = 0; i < renderableCollider.componentRanges.size(); i++) {
		auto range = renderableCollider.componentRanges[i];
		auto color = colors[i % (sizeof(colors) / sizeof(colors[0]))];

		if (collidedComponents.find(i) != collidedComponents.end()) {
			color = Color(1, 0, 0);
		}
		colliderShader->SetUniform("u_Color", (glm::vec3) color);
		OpenGL::DrawArrays(renderableCollider.vao, OpenGL::POINTS, range.first, range.second - 1);
	}

	glPointSize(1.0f);
}

void Renderer::RenderBVolume(const BVolume &bVolume, const Color &color) {
	PrimitiveBody body(PrimitiveBody::Type::SHAPED, color);

	switch (bVolume.TypeTag) {
		case BVolume::AABB: {
				auto &aabb = reinterpret_cast<const AABB&>(bVolume);
				Primitive3DProps props(aabb.GetPosition(), glm::vec3(0), aabb.GetWidths(), body);
				RenderCube(CubeProps(1, props));
			}
			break;
		case BVolume::OBB: {
				auto &obb = reinterpret_cast<const OBB&>(bVolume);
				Primitive3DProps props(obb.GetPosition(), obb.GetRotation(), obb.GetWidths(), body);
				RenderCube(CubeProps(1, props));
			}
			break;
		case BVolume::Sphere: {
				auto &sphere = reinterpret_cast<const Sphere&>(bVolume);
				Primitive3DProps props(sphere.GetPosition(), glm::vec3(0), glm::vec3(1), body);
				RenderSphere(SphereProps(sphere.GetRadius(), props));
			}
			break;
		default:
			break;
	}
}

void Renderer::SubmitModelForRendering(const Model *model, const glm::mat4 &modelMatrix) {
	modelRenderingQueue.emplace_back(model, modelMatrix);
}

void Renderer::ClearLights() {
	lightSources.clear();
}

void Renderer::AddLight(const LightSource &lightSource, const glm::vec3 &position) {
	lightSources.emplace_back(lightSource, position);
}
