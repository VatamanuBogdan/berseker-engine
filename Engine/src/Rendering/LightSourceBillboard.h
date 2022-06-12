#pragma once
#include "Rendering/GraphicsAPI/VertexArray.hpp"
#include "Rendering/GraphicsAPI/VertexBuffer.hpp"
#include "Rendering/GraphicsAPI/IndexBuffer.hpp"
#include "Rendering/GraphicsAPI/ShaderProgram.h"
#include "Rendering/GraphicsAPI/Texture2D.hpp"
#include "ResourceLoaders/ShaderRegistry.h"

#include <glm/glm.hpp>

#include <array>
#include <memory>
#include <optional>

class LightSourceBillboard {
public:
	LightSourceBillboard();

	void Draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);

private:
	static void Init();

private:
	static std::shared_ptr<ShaderProgram>	shader;
	static std::optional<VertexArray>		quadVao;
	static std::optional<IndexBuffer>		quadIbo;
	static std::optional<Texture2D>		texture;
};