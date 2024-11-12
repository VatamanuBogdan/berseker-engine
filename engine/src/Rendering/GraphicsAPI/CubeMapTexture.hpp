#pragma once
#include "OpenGL.h"

#include <stb_image.h>

#include <array>

struct CubeMapResource {
	const char *PositiveXTexturePath, *NegativeXTexturePath;
	const char *PositiveYTexturePath, *NegativeYTexturePath;
	const char *PositiveZTexturePath, *NegativeZTexturePath;

	CubeMapResource(const char *positiveXTexturePath, const char *negativeXTexturePath,
			    const char *positiveYTexturePath, const char *negativeYTexturePath,
			    const char *positiveZTexturePath, const char *negativeZTexturePath)
		: PositiveXTexturePath(positiveXTexturePath), NegativeXTexturePath(negativeXTexturePath),
		  PositiveYTexturePath(positiveYTexturePath), NegativeYTexturePath(negativeYTexturePath),
		  PositiveZTexturePath(positiveZTexturePath), NegativeZTexturePath(negativeZTexturePath) {}
};


class CubeMapTexture {
public:
	explicit CubeMapTexture(const CubeMapResource &resource) {
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

		std::array<const char*, 6> facesPaths = {
			  resource.PositiveXTexturePath,
			  resource.NegativeXTexturePath,
			  resource.PositiveYTexturePath,
			  resource.NegativeYTexturePath,
			  resource.PositiveZTexturePath,
			  resource.NegativeZTexturePath
		};

		uint8_t *data;
		int width, height, channels;
		for(int i = 0; i < 6; i++) {
			data = stbi_load(facesPaths[i], &width, &height, &channels, 0);
			glTexImage2D(
				  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				  0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void Bind() const {
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
	}

	static void Unbind() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

private:
	unsigned int handle;
};