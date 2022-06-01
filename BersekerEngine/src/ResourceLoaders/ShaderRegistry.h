#pragma once
#include "Rendering/GraphicsAPI/ShaderProgram.h"
#include "ShaderResources.h"

#include <memory>
#include <unordered_map>


class ShaderRegistry final {
public:
	static ShaderRegistry& Get();

	std::shared_ptr<ShaderProgram> GetShader(ShaderResource identifier);

private:
	ShaderRegistry() = default;
	ShaderRegistry(const ShaderRegistry &) = default;
	ShaderRegistry& operator=(const ShaderRegistry&) = default;

	static void Init();

private:
	std::unordered_map<ShaderResource, std::shared_ptr<ShaderProgram>> shaders;
	static std::unique_ptr<ShaderRegistry> instance;
};

