#include "FrameBuffer.h"
#include "OpenGL.hpp"

#include <cassert>

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
	: width(width), height(height) {
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	glGenTextures(1, &colorTextureHandle);
	glBindTexture(GL_TEXTURE_2D, colorTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureHandle, 0);

	glGenRenderbuffers(1, &rboHandle);
	glBindRenderbuffer(GL_RENDERBUFFER, rboHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboHandle);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Wrong framebuffer build");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(FrameBuffer &&fbo) noexcept
	: width(fbo.width), height(fbo.height), fboHandle(fbo.fboHandle),
	  rboHandle(fbo.rboHandle), colorTextureHandle(fbo.colorTextureHandle) {
	fbo.width = fbo.height = 0;
	fbo.fboHandle = fbo.rboHandle = fbo.colorTextureHandle = 0;
}

FrameBuffer::~FrameBuffer() {
	// TODO Read about OpenGL resource destruction
	glDeleteFramebuffers(1, &fboHandle);
}

void FrameBuffer::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
}

void FrameBuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::GetWidth() const {
	return width;
}

unsigned int FrameBuffer::GetHeight() const {
	return height;
}

unsigned int FrameBuffer::GetTexture() const {
	return colorTextureHandle;
}
