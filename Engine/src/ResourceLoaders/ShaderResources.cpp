#include "ShaderResources.h"

#include <type_traits>

ShaderSourceFiles ShaderResources::shaderSourceFiles[] = {
	ShaderSourceFiles("res/shaders/Model3D.vert.glsl", "res/shaders/Model3D.frag.glsl"),
	ShaderSourceFiles("res/shaders/Primitive.vert.glsl", "res/shaders/Primitive.frag.glsl"),
	ShaderSourceFiles("res/shaders/LightedModel.vert.glsl", "res/shaders/LightedModel.frag.glsl"),
	ShaderSourceFiles("res/shaders/LightSource.vert.glsl", "res/shaders/LightSource.frag.glsl"),
	ShaderSourceFiles("res/shaders/GridLine.vert.glsl", "res/shaders/GridLine.frag.glsl"),
	ShaderSourceFiles("res/shaders/Skybox.vert.glsl", "res/shaders/Skybox.frag.glsl"),
	ShaderSourceFiles("res/shaders/Collider.vert.glsl", "res/shaders/Collider.frag.glsl"),

};

const ShaderSourceFiles& ShaderResources::Get(ShaderResource identifier) {
	auto index = static_cast<std::underlying_type<ShaderResource>::type>(identifier);
	return shaderSourceFiles[index];
}
