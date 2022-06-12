#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <string_view>
#include <map>

class ShaderProgram {
public:
	ShaderProgram(std::string_view vertexShaderCode, std::string_view fragmentShaderCode);

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	ShaderProgram(ShaderProgram &&shader) noexcept;
	static ShaderProgram LoadFrom(const std::filesystem::path &vertexShaderPath,
						  const std::filesystem::path &fragmentShaderPath);

	void Bind() const;
	static void Unbind();

	void SetUniform(const char *uniformName, int value);
	void SetUniform(const char *uniformName, float value);

	void SetUniform(const char *uniformName, const glm::vec1 &vec);
	void SetUniform(const char *uniformName, const glm::vec2 &vec);
	void SetUniform(const char *uniformName, const glm::vec3 &vec);
	void SetUniform(const char *uniformName, const glm::vec4 &vec);

	void SetUniform(const char *uniformName, const glm::mat4 &mat);

protected:
	enum ShaderType : GLenum {
		VERTEX_SHADER = GL_VERTEX_SHADER,
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER
	};

	[[nodiscard]] static GLuint CreateShaderFrom(std::string_view sourceCode, ShaderType type);
	[[nodiscard]] static GLuint CreateShaderProgramFrom(std::string_view vertexShaderCode,
									    std::string_view fragmentShaderCode);

	GLint GetUniformLocation(const char *uniformName);

private:
	GLuint handle;
	std::map<std::string, GLint, std::less<>> uniformRegistry;
};
