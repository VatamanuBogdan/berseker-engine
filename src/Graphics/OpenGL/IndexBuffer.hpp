#pragma once
#include "OpenGL.hpp"

#include <vector>
#include <array>
#include <cstddef>

template <typename IndexType>
class IndexBuffer {
public:
	explicit IndexBuffer(const IndexType *indexes, size_t size, OpenGL::Primitive primitiveType = OpenGL::TRIANGLES)
		: size(size) {
		static_assert(
			  std::is_same_v<IndexType, GLubyte> ||
			  std::is_same_v<IndexType, GLuint> ||
			  std::is_same_v<IndexType, GLushort>,
			  "[Compile Error]::IndexType template typename of IndexBuffer class must be "
			  "one of the next types: GLubyte, GLushort GLuint"
			  );
		this->primitiveType = primitiveType;
		glGenBuffers(1, &handle);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexType) * size, indexes, GL_STATIC_DRAW);
		Unbind();
	}

	explicit IndexBuffer(const std::vector<IndexType> &indexes)
		: IndexBuffer(indexes.data(), indexes.size()) {
	}

	template <size_t N>
	explicit IndexBuffer(const std::array<IndexType , N> &indexes)
		: IndexBuffer(indexes.data(), indexes.size()) {
	}

	IndexBuffer(const std::initializer_list<IndexType> &indexes)
		: IndexBuffer(std::data(indexes), indexes.size()) {
	}

	IndexBuffer(IndexBuffer&& ibo) noexcept {
		handle = ibo.handle;
		size = ibo.size;
		primitiveType = ibo.primitiveType;
		ibo.handle = 0;
		ibo.size = 0;
	}

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	~IndexBuffer() {
		glDeleteBuffers(1, &handle);
	}

	void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle); }
	void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	[[nodiscard]] OpenGL::Numeric GetIndexType() const noexcept { return OpenGL::GetNumericFrom<IndexType>(); }

	[[nodiscard]] OpenGL::Primitive GetPrimitiveType() const noexcept { return primitiveType; }

	[[nodiscard]] size_t GetSize() const noexcept { return size; }

private:
	size_t 		size;
	unsigned int 	handle = 0;
	OpenGL::Primitive primitiveType;
};
