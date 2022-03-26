#pragma once
#include <stb_image.h>
#include <glad/glad.h>

#include <filesystem>

class Texture2D {
public:
	explicit Texture2D(const std::filesystem::path &texturePath) {
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		uint8_t *imageBlob = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
		if (imageBlob) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBlob);
		}
		stbi_image_free(imageBlob);
		stbi_set_flip_vertically_on_load(0);
	}

	Texture2D(Texture2D &&texture) noexcept {
		handle = texture.handle;
		texture.handle = 0;
	}

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	~Texture2D() {
		glDeleteTextures(1, &handle);
	}

	void Bind(unsigned short textureSlot) const {
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, handle);
	}

	static void Unbind(unsigned short textureSlot) {
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	GLuint handle = 0;
};