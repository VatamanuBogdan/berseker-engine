#pragma once
#include "OpenGL.hpp"

#include <vector>
#include <array>
#include <cstddef>


class IndexBuffer {
public:
	template <typename IndexType>
	explicit IndexBuffer(const IndexType *indexes, size_t size,
				   OpenGL::Primitive primitiveType = OpenGL::TRIANGLES)
		: size(size), indexDataType(OpenGL::GetDataTypeFrom<IndexType>()), primitiveType(primitiveType) {
		static_assert(
			  std::is_same_v<IndexType, GLubyte> ||
			  std::is_same_v<IndexType, GLuint> ||
			  std::is_same_v<IndexType, GLushort>,
			  "[Compile Error]::IndexType template typename of IndexBuffer class must be "
			  "one of the next types: GLubyte, GLushort GLuint"
			  );
		glGenBuffers(1, &handle);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexType) * size, indexes, GL_STATIC_DRAW);
		Unbind();
	}

	template <typename IndexType>
	explicit IndexBuffer(const std::vector<IndexType> &indexes,
				   OpenGL::Primitive primitiveType = OpenGL::TRIANGLES)
		: IndexBuffer(indexes.data(), indexes.size(), primitiveType) {
	}

	template <typename IndexType, size_t N>
	explicit IndexBuffer(const std::array<IndexType , N> &indexes,
				   OpenGL::Primitive primitiveType = OpenGL::TRIANGLES)
		: IndexBuffer(indexes.data(), indexes.size(), primitiveType) {
	}

	template <typename IndexType>
	IndexBuffer(const std::initializer_list<IndexType> &indexes,
			OpenGL::Primitive primitiveType = OpenGL::TRIANGLES)
		: IndexBuffer(std::data(indexes), indexes.size(), primitiveType) {
	}

	IndexBuffer(IndexBuffer&& ibo) noexcept
		: handle(ibo.handle), size(ibo.size), indexDataType(ibo.indexDataType),
		  primitiveType(ibo.primitiveType) {
		ibo.handle = 0;
		ibo.size = 0;
	}

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	~IndexBuffer() {
		glDeleteBuffers(1, &handle);
	}

	void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle); }
	static void Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	[[nodiscard]] OpenGL::DataType GetIndexType() const noexcept { return indexDataType; }

	[[nodiscard]] OpenGL::Primitive GetPrimitiveType() const noexcept { return primitiveType; }

	[[nodiscard]] size_t GetSize() const noexcept { return size; }

private:
	size_t 		size;
	unsigned int 	handle = 0;
	OpenGL::DataType	indexDataType;
	OpenGL::Primitive primitiveType;
};

namespace OpenGL {
	inline void DrawElementsWith(const IndexBuffer &ibo) {
		ibo.Bind();
		glDrawElements(ibo.GetPrimitiveType(), ibo.GetSize(), ibo.GetIndexType(), (void*) 0);
		IndexBuffer::Unbind();
	}

	inline void DrawElementsWith(const IndexBuffer &ibo, size_t startIndex, size_t endIndex) {
		ibo.Bind();
		glDrawElements(ibo.GetPrimitiveType(), endIndex - startIndex + 1, ibo.GetIndexType(), (void*) startIndex);
		IndexBuffer::Unbind();
	}
}
