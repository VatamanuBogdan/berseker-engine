#pragma once

class UIRendererBackend {
public:
	virtual void Init() = 0;
	virtual void Deinit() = 0;

	virtual void PreUIRendering() = 0;
	virtual void PostUIRendering() = 0;
};