#include "Renderer.h"
#include "Rendering/GraphicsAPI/OpenGL.hpp"
#include "Rendering/GraphicsAPI/ShaderProgram.h"

#include <memory>

// TODO Replace temporary workaround
ShaderProgram		*cubeShader;

void PrimitivesRenderer::Init() {
	cubeShader = new ShaderProgram(
		  ShaderProgram::LoadFrom(
		    "res/shaders/Primitive.vert.glsl",
		    "res/shaders/Primitive.frag.glsl"
		    ));
	InitCubeGeometry();
	InitSphereGeometry();
}

void PrimitivesRenderer::Deinit() {
	// TODO
}

void PrimitivesRenderer::AttachCamera(std::shared_ptr<Camera> &camera) {
	this->camera = camera;
}

void PrimitivesRenderer::RenderCube(const CubeProps &props) {
	Mesh *mesh;
	switch (props.BodyType.Type) {
		case PrimitiveBody::FILLED:
			mesh = &storage.GetGeometry(PrimitivesStorage::FILLED_CUBE_GEOMETRY);
			break;
		case PrimitiveBody::SHAPED:
			mesh = &storage.GetGeometry(PrimitivesStorage::SHAPED_CUBE_GEOMETRY);
			break;
		default:
			return;
	}

	glm::mat4 model = glm::translate(glm::mat4(1), props.Position);
	model = glm::rotate(model, props.Rotation.z, glm::vec3(0, 0, 1));
	model = glm::rotate(model, props.Rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, props.Rotation.x, glm::vec3(1, 0, 0));
	model = glm::scale(model, props.Scale * glm::vec3(props.Width));

	cubeShader->Bind();
	cubeShader->SetUniform("Projection", camera->GetProjection());
	cubeShader->SetUniform("Model", model);
	cubeShader->SetUniform("View", camera->GetView());
	cubeShader->SetUniform("Color", static_cast<glm::vec4>(props.BodyType.BodyColor));
	mesh->GetVertexArray().Bind();
	OpenGL::SetLineWidth(props.BodyType.EdgeWidth);
	glEnable(GL_LINE_SMOOTH);
	OpenGL::DrawElementsWith(mesh->GetIndexBuffer());
	OpenGL::SetLineWidth(1.0f);
}

void PrimitivesRenderer::RenderSphere(const SphereProps &props) {
	Mesh *mesh;
	switch (props.BodyType.Type) {
		case PrimitiveBody::FILLED:
			// TODO Implemented this
			throw std::runtime_error("NOT IMPLEMENTED");
			break;
		case PrimitiveBody::SHAPED:
			mesh = &storage.GetGeometry(PrimitivesStorage::SHAPED_SPHERE_GEOMETRY);
			break;
		default:
			return;
	}

	glm::mat4 model = glm::translate(glm::mat4(1), props.Position);
	model = glm::rotate(model, props.Rotation.z, glm::vec3(0, 0, 1));
	model = glm::rotate(model, props.Rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, props.Rotation.x, glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(props.Radius));

	cubeShader->Bind();
	cubeShader->SetUniform("Projection", camera->GetProjection());
	cubeShader->SetUniform("Model", model);
	cubeShader->SetUniform("View", camera->GetView());
	cubeShader->SetUniform("Color", static_cast<glm::vec4>(props.BodyType.BodyColor));
	mesh->GetVertexArray().Bind();
	OpenGL::SetLineWidth(props.BodyType.EdgeWidth);
	glEnable(GL_LINE_SMOOTH);
	OpenGL::DrawElementsWith(mesh->GetIndexBuffer());
	OpenGL::SetLineWidth(1.0f);
}


void PrimitivesRenderer::InitCubeGeometry() {
	std::array<float, 24> vertices = {
		  0.5, -0.5, -0.5,
		  0.5, -0.5, 0.5,
		  0.5, 0.5, 0.5,
		  0.5, 0.5, -0.5,
		  -0.5, -0.5, -0.5,
		  -0.5, -0.5, 0.5,
		  -0.5, 0.5, 0.5,
		  -0.5, 0.5, -0.5
	};

	VertexFormat<1> format;
	format.Push<float>(3, 0);

	{ // SHAPED CUBE GEOMETRY
		std::array<unsigned, 16> indices = {
			  0, 1, 2, 3,
			  0, 4, 7, 3,
			  2, 6, 7, 4,
			  5, 6, 5, 1
		};

		storage.SetGeometry(
			  PrimitivesStorage::SHAPED_CUBE_GEOMETRY,
			  Mesh(format, VertexBuffer(vertices), IndexBuffer(indices, OpenGL::LINE_STRIP)
			  ));
	}
	{ // FILLED CUBE GEOMETRY
		std::array<unsigned, 36> indices = {
			  0, 1, 2, 0, 3, 2,
			  4, 5, 6, 6, 4, 7,
			  0, 4, 5, 5, 1, 0,
			  3, 7, 6, 6, 3, 2,
			  3, 0, 4, 4, 7, 3,
			  2, 6, 5, 5, 1, 2
		};

		storage.SetGeometry(
			  PrimitivesStorage::FILLED_CUBE_GEOMETRY,
			  Mesh(format, VertexBuffer(vertices), IndexBuffer(indices, OpenGL::TRIANGLES)
			  ));
	}
}

void PrimitivesRenderer::InitSphereGeometry() {
	constexpr unsigned stacksNum = 64;
	constexpr unsigned sectorNum = 64;

	constexpr unsigned verticesNum = stacksNum /* xy plane circle */
					+ sectorNum; /* xz plane circle */
	constexpr unsigned indicesNum = stacksNum
		  			+ 1 /* xy plane circle connection vertex */
					+ sectorNum
					+ 1; /* xz plane circle connection vertex */

	std::array<glm::vec3, verticesNum> vertices{};
	std::array<unsigned, indicesNum> indices{};

	for (unsigned i = 0; i < stacksNum; i++) {
		const float angle = 2 * glm::pi<float>() * static_cast<float>(i) / stacksNum;
		vertices[i].x = glm::cos(angle);
		vertices[i].y = glm::sin(angle);
		vertices[i].z = 0;

		indices[i] = i;
	}
	indices[stacksNum] = 0;

	for (unsigned i = 0; i < sectorNum; i++) {
		const unsigned ri = stacksNum + i;
		const float angle = 2 * glm::pi<float>() * static_cast<float>(i) / sectorNum;
		vertices[ri].x = glm::cos(angle);
		vertices[ri].y = 0;
		vertices[ri].z = glm::sin(angle);

		indices[ri + 1] = ri;
	}
	indices[stacksNum + 1 + sectorNum] = stacksNum;

	VertexFormat<1> format;
	format.Push<float>(3, 0);

	storage.SetGeometry(
		  PrimitivesStorage::SHAPED_SPHERE_GEOMETRY,
		  Mesh(format, VertexBuffer(vertices), IndexBuffer(indices, OpenGL::LINE_STRIP))
	);
}

