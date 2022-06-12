#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram(std::string_view vertexShaderCode, std::string_view fragmentShaderCode) {
	handle = CreateShaderProgramFrom(vertexShaderCode, fragmentShaderCode);
}

ShaderProgram::ShaderProgram(ShaderProgram &&shader) noexcept{
	handle = shader.handle;
	shader.handle = 0;
}

// TODO Rewrite this shit
ShaderProgram ShaderProgram::LoadFrom(const std::filesystem::path &vertexShaderPath,
						    const std::filesystem::path &fragmentShaderPath) {
	using namespace std::string_literals;

	std::fstream fin;
	std::stringstream stream1;
	std::stringstream stream2;

	fin.open(vertexShaderPath, std::ios_base::in);
	if (!fin.is_open()) {
		throw std::runtime_error("Cannot open file "s + vertexShaderPath.c_str());
	}
	stream1 << fin.rdbuf();
	std::string vertexShaderCode = stream1.str();
	fin.close();

	fin.open(fragmentShaderPath, std::ios_base::in);
	if (!fin.is_open()) {
		throw std::runtime_error("Cannot open file "s + fragmentShaderPath.c_str());
	}
	stream2 << fin.rdbuf();
	std::string fragmentShaderCode = stream2.str();
	fin.close();

	return {vertexShaderCode, fragmentShaderCode};
}

void ShaderProgram::Bind() const {
	glUseProgram(handle);
}

void ShaderProgram::Unbind() {
	glUseProgram(0);
}

void ShaderProgram::SetUniform(const char *uniformName, float value) {
	glUniform1f(GetUniformLocation(uniformName), value);
}

void ShaderProgram::SetUniform(const char *uniformName, const glm::vec1 &vec) {
	glUniform1f(GetUniformLocation(uniformName), vec.x);
}

void ShaderProgram::SetUniform(const char *uniformName, const glm::vec2 &vec) {
	glUniform2f(GetUniformLocation(uniformName), vec.x, vec.y);
}

void ShaderProgram::SetUniform(const char *uniformName, const glm::vec3 &vec) {
	glUniform3f(GetUniformLocation(uniformName), vec.x, vec.y, vec.z);
}

void ShaderProgram::SetUniform(const char *uniformName, const glm::vec4 &vec) {
	glUniform4f(GetUniformLocation(uniformName), vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::SetUniform(const char *uniformName, const glm::mat4 &mat) {
	glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
}

GLuint ShaderProgram::CreateShaderFrom(std::string_view sourceCode, ShaderProgram::ShaderType type) {
	GLuint shaderHandle = glCreateShader(type);
	const char *shaderArgument[] = {sourceCode.data()};
	auto sourceCodeSize = static_cast<GLint>(sourceCode.size());

	glShaderSource(shaderHandle, 1, shaderArgument, &sourceCodeSize);
	glCompileShader(shaderHandle);

	GLint resultCode;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &resultCode);
	if (resultCode == GL_FALSE) {
		GLint errorMessageLength;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &errorMessageLength);
		char errorMessage[errorMessageLength];
		glGetShaderInfoLog(shaderHandle, errorMessageLength, &errorMessageLength, errorMessage);
		glDeleteShader(shaderHandle);

		throw std::runtime_error(errorMessage);
	}

	return shaderHandle;
}

GLuint ShaderProgram::CreateShaderProgramFrom(std::string_view vertexShaderCode, std::string_view fragmentShaderCode) {
	GLuint programHandle = glCreateProgram();
	GLuint vertexShaderHandle = 0;
	GLuint fragmentShaderHandle = 0;

	try {
		vertexShaderHandle = CreateShaderFrom(vertexShaderCode, VERTEX_SHADER);
		fragmentShaderHandle = CreateShaderFrom(fragmentShaderCode, FRAGMENT_SHADER);
	} catch (std::runtime_error &err) {
		glDeleteShader(vertexShaderHandle);
		glDeleteShader(fragmentShaderHandle);
		glDeleteProgram(programHandle);
		throw err;
	}

	glAttachShader(programHandle, vertexShaderHandle);
	glAttachShader(programHandle, fragmentShaderHandle);
	glLinkProgram(programHandle);
	glValidateProgram(programHandle);

	glDeleteShader(vertexShaderHandle);
	glDeleteShader(fragmentShaderHandle);

	return programHandle;
}

GLint ShaderProgram::GetUniformLocation(const char *uniformName) {
	auto iter = uniformRegistry.find(uniformName);
	if (iter != uniformRegistry.end()) {
		return iter->second;
	}
	GLint uniformLocation = glGetUniformLocation(handle, uniformName);
	uniformRegistry.insert({uniformName, uniformLocation});
	return uniformLocation;
}