#include "ShaderResources.h"

#include <type_traits>

ShaderSourceFiles ShaderResources::shaderSourceFiles[] = {
	ShaderSourceFiles("resources/shaders/Model3D.vert.glsl", "resources/shaders/Model3D.frag.glsl"),
	ShaderSourceFiles("resources/shaders/Primitive.vert.glsl", "resources/shaders/Primitive.frag.glsl"),
	ShaderSourceFiles("resources/shaders/LightedModel.vert.glsl", "resources/shaders/LightedModel.frag.glsl"),
	ShaderSourceFiles("resources/shaders/LightSource.vert.glsl", "resources/shaders/LightSource.frag.glsl"),
	ShaderSourceFiles("resources/shaders/GridLine.vert.glsl", "resources/shaders/GridLine.frag.glsl"),
	ShaderSourceFiles("resources/shaders/Skybox.vert.glsl", "resources/shaders/Skybox.frag.glsl"),
	ShaderSourceFiles("resources/shaders/Collider.vert.glsl", "resources/shaders/Collider.frag.glsl"),

};

const ShaderSourceFiles& ShaderResources::Get(ShaderResource identifier) {
	auto index = static_cast<std::underlying_type<ShaderResource>::type>(identifier);
	return shaderSourceFiles[index];
}
