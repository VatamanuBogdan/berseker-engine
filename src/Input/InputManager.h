#pragma once
#include "InputKeys.h"

enum class KeyState {
	Pressed, Released
};

struct MousePosition {
	double xPosition;
	double yPosition;

	explicit MousePosition(double xPosition, double yPosition)
		: xPosition(xPosition), yPosition(yPosition) {
	}
};

class InputManager {
public:
	[[nodiscard]] virtual KeyState GetStateForKey(KeyCodeType keyCode) const = 0;
	[[nodiscard]] virtual MousePosition GetMousePosition() const = 0;
	[[nodiscard]] virtual KeyState GetMouseButtonState(KeyCodeType buttonCode) const = 0;

protected:
	InputManager() = default;
};