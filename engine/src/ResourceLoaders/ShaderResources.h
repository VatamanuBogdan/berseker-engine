struct ShaderSourceFiles {
	const char * const VertexShaderPath;
	const char * const FragmentShaderPath;

	ShaderSourceFiles(const char *vertexShaderPath, const char *fragmentShaderPath) noexcept
		: VertexShaderPath(vertexShaderPath),FragmentShaderPath(fragmentShaderPath) {
	}
};


enum class ShaderResource {
	Model3D,
	Primitive,
	LightedModel,
	LightSource,
	GridLine,
	Skybox,
	Collider,

};


class ShaderResources {
public:
	ShaderResources() = delete;
	static const ShaderSourceFiles& Get(ShaderResource identifier);

private:
	static ShaderSourceFiles shaderSourceFiles[];
};
