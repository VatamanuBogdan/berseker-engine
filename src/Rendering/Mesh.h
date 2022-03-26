#pragma once
#include "Rendering/GraphicsAPI/VertexArray.hpp"
#include "Rendering/GraphicsAPI/VertexBuffer.hpp"
#include "Rendering/GraphicsAPI/IndexBuffer.hpp"
#include "Utils.h"

#include <glm/glm.hpp>

#include <string>

class Mesh {
public:
	template <unsigned short AttributesNum>
	Mesh(const VertexFormat<AttributesNum> &attributes, VertexBuffer &&vbo, IndexBuffer<unsigned int> &&ibo)
		: vbo(std::move(vbo)), ibo(std::move(ibo)) {
		vao.template AttachVertexBuffer(this->vbo, attributes);
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

	[[nodiscard]] const IndexBuffer<unsigned int>& GetIndexBuffer() const noexcept { return ibo; };
	[[nodiscard]] IndexBuffer<unsigned int>& GetIndexBuffer() noexcept { return ibo; };

private:
	DebugClassMember(std::string, name);
	VertexArray 			vao;
	VertexBuffer 			vbo;
	IndexBuffer<unsigned int> 	ibo;
};