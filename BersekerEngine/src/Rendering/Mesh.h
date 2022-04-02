#pragma once
#include "Rendering/GraphicsAPI/VertexArray.hpp"
#include "Rendering/GraphicsAPI/VertexBuffer.hpp"
#include "Rendering/GraphicsAPI/IndexBuffer.hpp"
#include "../Utils.h"

#include <glm/glm.hpp>

#include <string>

class Mesh {
public:
	template <unsigned short AttributesNum>
	Mesh(const VertexFormat<AttributesNum> &attributes, VertexBuffer &&vbo, IndexBuffer &&ibo)
		: vbo(std::move(vbo)), ibo(std::move(ibo)) {
		vao.template AttachVertexBuffer(this->vbo, attributes);
	}

	Mesh(Mesh &&mesh) noexcept
		: vao(std::move(mesh.vao)), vbo(std::move(mesh.vbo)), ibo(std::move(mesh.ibo)) {
		OnDebug(name = std::move(mesh.name));
	}

	OnDebug(
		void SetName(const char *name) {
			this->name = name;
		}
	)

	template <unsigned short AttributesNum>
	void SetVertexAttributes(VertexFormat<AttributesNum> attributes) {
		vao.template AttachVertexBuffer(this->vbo, attributes);
	}
	[[nodiscard]] const VertexArray& GetVertexArray() const noexcept { return vao; };

	[[nodiscard]] const VertexBuffer& GetVertexBuffer() const noexcept { return vbo; };
	[[nodiscard]] VertexBuffer& GetVertexBuffer() noexcept { return vbo; };

	[[nodiscard]] const IndexBuffer& GetIndexBuffer() const noexcept { return ibo; };
	[[nodiscard]] IndexBuffer& GetIndexBuffer() noexcept { return ibo; };

private:
	DebugClassMember(std::string, name);
	VertexArray 			vao;
	VertexBuffer 			vbo;
	IndexBuffer				ibo;
};