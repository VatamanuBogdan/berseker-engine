#pragma once
#include <glad/glad.h>

#include <vector>
#include <array>
#include <cstddef>

class VertexBuffer {
public:
	template<typename T>
	explicit VertexBuffer(const T *data, size_t size) {
		glGenBuffers(1, &handle);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(T) * size, data, GL_STATIC_DRAW);
		Unbind();
	}

	template <typename T>
	explicit VertexBuffer(const std::vector<T> &data)
		: VertexBuffer(data.data(), data.size()) {
	}

	template <typename T, size_t N>
	explicit VertexBuffer(const std::array<T, N> &data)
		: VertexBuffer(data.data(), data.size()) {
	}

	template <typename T>
	VertexBuffer(const std::initializer_list<T> &data)
		: VertexBuffer(std::data(data), data.size()) {
	}

	VertexBuffer(VertexBuffer&& vbo) noexcept {
		handle = vbo.handle;
		vbo.handle = 0;
	}

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	~VertexBuffer() {
		glDeleteBuffers(1, &handle);
	}

	void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, handle); }
	static void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

private:
	GLuint handle = 0;
};
