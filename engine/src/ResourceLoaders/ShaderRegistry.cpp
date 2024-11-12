#include "ShaderRegistry.h"


std::unique_ptr<ShaderRegistry> ShaderRegistry::instance = nullptr;

ShaderRegistry &ShaderRegistry::Get() {
	if (!instance) {
		Init();
	}
	return *instance;
}

void ShaderRegistry::Init() {
	instance.reset(new ShaderRegistry());
}

std::shared_ptr<ShaderProgram> ShaderRegistry::GetShader(ShaderResource identifier) {
	auto entry = shaders.find(identifier);
	if (entry != shaders.end()) {
		return entry->second;
	}

	ShaderSourceFiles resource = ShaderResources::Get(identifier);
	auto result = shaders.insert(
		  {
			  identifier,
			  std::make_shared<ShaderProgram>(
				    ShaderProgram::LoadFrom(resource.VertexShaderPath, resource.FragmentShaderPath)
				)
		  }
	);

	return result.first->second;
}
