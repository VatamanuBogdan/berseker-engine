#pragma once
#include <glad/glad.h>

#include <cstddef>
#include <type_traits>

class IndexBuffer;

namespace OpenGL {

	enum DataType {
		BYTE 			= GL_BYTE,
		UNSIGNED_BYTE 	= GL_UNSIGNED_BYTE,
		SHORT 		= GL_SHORT,
		UNSIGNED_SHORT 	= GL_UNSIGNED_SHORT,
		INT 			= GL_INT,
		UNSIGNED_INT 	= GL_UNSIGNED_INT,
		FLOAT 		= GL_FLOAT,
	};

	template <typename CanonicalDataType>
	constexpr DataType GetDataTypeFrom() noexcept {
		if constexpr(std::is_same_v<CanonicalDataType, char>) {
			return DataType::BYTE;
		} else if constexpr(std::is_same_v<CanonicalDataType, unsigned char>) {
			return DataType::UNSIGNED_BYTE;
		} else if constexpr(std::is_same_v<CanonicalDataType, short>) {
			return DataType::SHORT;
		} else if constexpr(std::is_same_v<CanonicalDataType, unsigned short>) {
			return DataType::UNSIGNED_SHORT;
		} if constexpr(std::is_same_v<CanonicalDataType, int>) {
			return DataType::INT;
		} else if constexpr(std::is_same_v<CanonicalDataType, unsigned int>) {
			return DataType::UNSIGNED_INT;
		} else if constexpr(std::is_same_v<CanonicalDataType, float>) {
			return DataType::FLOAT;
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

	inline void SetLineWidth(float width) {
		glLineWidth(width);
	}
}