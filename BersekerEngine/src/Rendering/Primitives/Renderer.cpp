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

	cubeShader->Bind();
	cubeShader->SetUniform("Projection", camera->GetProjection());
	cubeShader->SetUniform("Model", glm::mat4(1.0f));
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

