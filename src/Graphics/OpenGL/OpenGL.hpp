#pragma once
#include <glad/glad.h>

#include <cstddef>
#include <type_traits>

template <typename IndexType>
class IndexBuffer;

namespace OpenGL {

	enum Numeric {
		BYTE 			= GL_BYTE,
		UNSIGNED_BYTE 	= GL_UNSIGNED_BYTE,
		SHORT 		= GL_SHORT,
		UNSIGNED_SHORT 	= GL_UNSIGNED_SHORT,
		INT 			= GL_INT,
		UNSIGNED_INT 	= GL_UNSIGNED_INT,
		FLOAT 		= GL_FLOAT,
	};

	template <typename NumericType>
	constexpr Numeric GetNumericFrom() noexcept {
		if constexpr(std::is_same_v<NumericType, char>) {
			return Numeric::BYTE;
		} else if constexpr(std::is_same_v<NumericType, unsigned char>) {
			return Numeric::UNSIGNED_BYTE;
		} else if constexpr(std::is_same_v<NumericType, short>) {
			return Numeric::SHORT;
		} else if constexpr(std::is_same_v<NumericType, unsigned short>) {
			return Numeric::UNSIGNED_SHORT;
		} if constexpr(std::is_same_v<NumericType, int>) {
			return Numeric::INT;
		} else if constexpr(std::is_same_v<NumericType, unsigned int>) {
			return Numeric::UNSIGNED_INT;
		} else if constexpr(std::is_same_v<NumericType, float>) {
			return Numeric::FLOAT;
		} else {
			static_assert(" Invalid Numeric type");
		}
	}

	enum Primitive {
		POINTS				= GL_POINTS,
		LINES					= GL_LINES,
		LINE_LOOP				= GL_LINE_LOOP,
		LINE_STRIP				= GL_LINE_STRIP,
		LINES_ADJACENCY			= GL_LINES_ADJACENCY,
		LINE_STRIP_ADJACENCY 		= GL_LINE_STRIP_ADJACENCY,
		TRIANGLES				= GL_TRIANGLES,
		TRIANGLES_FAN 			= GL_TRIANGLE_FAN,
		TRIANGLES_STRIP 			= GL_TRIANGLE_STRIP,
		TRIANGLES_ADJACENCY 		= GL_TRIANGLES_ADJACENCY,
		TRIANGLES_STRIP_ADJACENCY 	= GL_TRIANGLE_STRIP_ADJACENCY,
		PATCHES				= GL_PATCHES
	};

	template <typename IndexType>
	void DrawElementsWith(const IndexBuffer<IndexType> &ibo) {
		ibo.Bind();
		glDrawElements(ibo.GetPrimitiveType(), ibo.GetSize(), ibo.GetIndexType(), (void*) 0);
		ibo.Unbind();
	}

	template <typename IndexType>
	void DrawElementsWith(const IndexBuffer<IndexType> &ibo, size_t startIndex, size_t endIndex) {
		ibo.Bind();
		glDrawElements(ibo.GetPrimitiveType(), endIndex - startIndex + 1, ibo.GetIndexType(), (void*) startIndex);
		ibo.Unbind();
	}
}