#pragma once

#include <vector>
#include <string>
#include <memory>

class View {
public:
	virtual void Draw() = 0;
};

class Layout : public View {
public:
	void Draw() override = 0;
};

class LinearLayout : public View {
public:
	explicit LinearLayout(const char *id);
	void Draw() override;

	void AddChildren(View *view);

private:
	std::vector<std::unique_ptr<View>>	views;
	std::string					id;
};