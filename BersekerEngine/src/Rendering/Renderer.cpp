#include "Rendering/Renderer.h"
#include "Rendering/LightSourceBillboard.h"

PrimitivesRenderer				Renderer::primitivesRender;
Color							Renderer::clearColor(0, 0, 0);

std::optional<Skybox>				Renderer::skybox;

LightSource						Renderer::lightSource(glm::vec3(0), glm::vec3(0), glm::vec3(0));
glm::vec3						Renderer::lightPosition;

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

	LightSourceBillboard().Draw(
		  glm::translate(glm::mat4(1), lightPosition),
		  Renderer::camera->GetView(),
		  Renderer::camera->GetProjection()
  	);

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
				shader->SetUniform("u_LightSource.Position", lightPosition);
				shader->SetUniform("u_LightSource.Ambient", lightSource.Ambient);
				shader->SetUniform("u_LightSource.Diffuse", lightSource.Diffuse);
				shader->SetUniform("u_LightSource.Specular", lightSource.Specular);

				shader->SetUniform("u_Material.Ambient", material.Ambient);
				shader->SetUniform("u_Material.Diffuse", material.Diffuse);
				shader->SetUniform("u_Material.Specular", material.Specular);
				shader->SetUniform("u_Material.Shininess", material.Shininess);
			}

			mesh.GetVertexArray().Bind();
			OpenGL::DrawElementsWith(mesh.GetIndexBuffer());
			materialIndex++;
		}

	}
	modelRenderingQueue.clear();
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

void Renderer::SubmitModelForRendering(const Model *model, const glm::mat4 &modelMatrix) {
	modelRenderingQueue.emplace_back(model, modelMatrix);
}

void Renderer::SetLight(const LightSource &lightSource, const glm::vec3 &lightPosition) {
	Renderer::lightSource = lightSource;
	Renderer::lightPosition = lightPosition;
}
