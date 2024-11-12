#include "OpenGL.h"

#include "Core/Logger.h"

namespace OpenGL {
	bool SetupOpenGL(GLADloadproc gladLoadProc, OpenGLDebugCallback callback) {
		if (!gladLoadGLLoader(gladLoadProc)) {
			return false;
		}

		if (callback != nullptr) {
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(callback, nullptr);
		}

		return true;
	}

	void SetLineWidth(float width) {
		glLineWidth(width);
	}
}