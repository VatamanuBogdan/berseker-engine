#pragma once

#include "Core/Layout.h"

#include <vector>
#include <string>
#include <memory>

class LinearLayout : public View {
public:
	explicit LinearLayout(const char *id);
	void Draw() override;

	void AddChildren(View *view);

private:
	std::vector<std::unique_ptr<View>>	views;
	std::string	id;
};