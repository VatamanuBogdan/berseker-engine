#pragma once
#include "VertexBuffer.hpp"

#include <glad/glad.h>

#include <vector>
#include <array>
#include <initializer_list>

class VertexArray {
public:
	class Attribute {
	public:
		template <typename AttrType>
		static Attribute Create(unsigned int location, unsigned int number, unsigned int stride,
						unsigned int offset, bool normalized = false) {
			Attribute result{};
			static_assert(
				  std::is_same_v<AttrType, float> ||
				  std::is_same_v<AttrType, double> ||
				    "[Compile Error]::AttrType template typename of VertexArray::Attribute class must be "
				    "one of the next types: float, double");

			if constexpr(std::is_same_v<AttrType, float>)
				result.type = GL_FLOAT;
			else
				result.type = GL_DOUBLE;

			result.location = location;
			result.number = number;
			result.stride = stride;
			result.offset = offset;
			result.normalized = normalized ? GL_TRUE : GL_FALSE;
			return result;
		}
	private:
		Attribute() = default;

		GLuint 	location;
		GLint 	number;
		GLenum 	type;
		GLboolean 	normalized;
		GLsizei	stride;
		GLsizeiptr 	offset;
		friend class VertexArray;
	};

public:
	VertexArray() {
		glGenVertexArrays(1, &handle);
	}

	VertexArray(VertexArray &&vao) noexcept {
		this->handle = vao.handle;
		vao.handle = 0;
	}

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	~VertexArray() {
		glDeleteVertexArrays(1, &handle);
	}

	// TODO Change to use forward iterators
	void AttachVertexBuffer(const VertexBuffer &vbo, const VertexArray::Attribute *attributes, size_t size) {
		Bind();
		vbo.Bind();
		for (size_t i = 0; i < size; i++) {
			glEnableVertexAttribArray(attributes[i].location);
			glVertexAttribPointer(
				  attributes[i].location,
				  attributes[i].number,
				  attributes[i].type,
				  attributes[i].normalized,
				  attributes[i].stride,
				  reinterpret_cast<void*>(attributes[i].offset)
				  );
		}
		vbo.Unbind();
		Unbind();
	}

	void AttachVertexBuffer(const VertexBuffer &vbo, const std::vector<VertexArray::Attribute> &attributes) {
		AttachVertexBuffer(vbo, attributes.data(), attributes.size());
	}

	void AttachVertexBuffer(const VertexBuffer &vbo, const std::initializer_list<VertexArray::Attribute> &attributes) {
		AttachVertexBuffer(vbo, std::data(attributes), attributes.size());
	}

	template <size_t N>
	void AttachVertexBuffer(const VertexBuffer &vbo, const std::array<VertexArray::Attribute, N> &attributes) {
		AttachVertexBuffer(vbo, attributes, N);
	}

	void Bind() const { glBindVertexArray(handle); }
	static void Unbind() { glBindVertexArray(0); }

private:
	GLuint handle = 0;
};


