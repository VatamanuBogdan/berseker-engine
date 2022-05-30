#pragma once
#include "Rendering/GraphicsAPI/VertexArray.hpp"
#include "Rendering/GraphicsAPI/IndexBuffer.hpp"
#include "Rendering/GraphicsAPI/ShaderProgram.h"
#include "Rendering/Mesh.h"
#include "Rendering/Camera.hpp"
#include "Rendering/Primitives/Renderer.h"
#include "Collision/BVolumes.h"
#include "Rendering/Model.h"
#include "ECS/Components/Material.hpp"
#include "LightSource.h"


#include <vector>

class Renderer {
public:
	static void Init();
	static void Deinit();
	static void SetCamera(std::shared_ptr<Camera> &camera);

	static void RenderCube(const CubeProps &props);
	static void RenderSphere(const SphereProps &props);
	static void RenderBVolume(const BVolumes::BVolume &bVolume, const Color &color);
	static void RenderClearColor(const Color &color);

	static void SubmitModelForRendering(const Model *model,const Material &material, const glm::mat4 &modelMatrix);
	static void Render();

	static void SetLight(const LightSource &lightSource, const glm::vec3 &position);

private:
	struct RenderingModel {
		RenderingModel(const Model *model, const Material &material, const glm::mat4 &modelMatrix)
			  : model(model), material(material), modelMatrix(modelMatrix) {}

		const Model	*model;
		Material	 material;
		glm::mat4	 modelMatrix;
	};

	Renderer() = default;

private:
	static PrimitivesRenderer 		primitivesRender;
	static Color				clearColor;

	static LightSource			lightSource;
	static glm::vec3				lightPosition;

	static std::shared_ptr<Camera> 	camera;
	static std::vector<RenderingModel>	modelRenderingQueue;
};

