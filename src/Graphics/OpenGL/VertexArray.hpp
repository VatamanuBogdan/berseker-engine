#pragma once
#include "OpenGL.hpp"
#include "VertexBuffer.hpp"

#include <vector>
#include <array>
#include <initializer_list>

template <unsigned short FormatsNum>
class VertexFormat {
public:
	template <typename DataType>
	VertexFormat& Push(unsigned int number, size_t offset, bool normalized = false) {
		static_assert(
			  std::is_same_v<DataType, float> ||
			  std::is_same_v<DataType, double> ||
			  "[Compile Error]::AttrType template typename of VertexArray::Attribute class must be "
			  "one of the next types: float, double");

		if constexpr(std::is_same_v<DataType, float>) {
			formats[locationPtr].type = GL_FLOAT;
		} else {
			formats[locationPtr].type = GL_DOUBLE;
		}

		formats[locationPtr].number = number;
		formats[locationPtr].normalized = normalized ? GL_TRUE : GL_FALSE;
		formats[locationPtr].offset = offset;
		strideAcc = offset + number * sizeof(DataType);
		locationPtr++;

		return *this;
	}

private:
	struct Attribute {
		GLenum 	type;
		GLint 	number;
		GLboolean 	normalized;
		GLsizeiptr 	offset;
	};
	unsigned short 	locationPtr = 0;
	GLuint 		strideAcc = 0;
	Attribute 		formats[FormatsNum];

private:
	friend class VertexArray;
};


class VertexArray {
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

	template <unsigned short AttributesMaxNum>
	void AttachVertexBuffer(const VertexBuffer &vbo, VertexFormat<AttributesMaxNum> attributes) {
		Bind();
		vbo.Bind();
		for (unsigned short location = 0; location < attributes.locationPtr; location++) {
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(
				  location,
				  attributes.formats[location].number,
				  attributes.formats[location].type,
				  attributes.formats[location].normalized,
				  attributes.strideAcc,
				  reinterpret_cast<void*>(attributes.formats[location].offset)
				  );
		}
		VertexBuffer::Unbind();
		Unbind();
	}

	void Bind() const { glBindVertexArray(handle); }
	static void Unbind() { glBindVertexArray(0); }

private:
	unsigned int handle = 0;
};


