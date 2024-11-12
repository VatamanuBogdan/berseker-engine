#pragma once

class FrameBuffer {
public:
	FrameBuffer(unsigned int width, unsigned int height);

	FrameBuffer(FrameBuffer &&fbo) noexcept;

	FrameBuffer(const FrameBuffer&) = delete;
	FrameBuffer& operator=(const FrameBuffer&) = delete;

	~FrameBuffer();

	void Bind() const;
	static void Unbind();

	[[nodiscard]] unsigned int GetWidth() const;
	[[nodiscard]] unsigned int GetHeight() const;

	[[nodiscard]] unsigned int GetTexture() const;

private:
	unsigned int fboHandle = 0;
	unsigned int rboHandle = 0;
	unsigned int colorTextureHandle = 0;
	unsigned int width;
	unsigned int height;
};

