#pragma once
#include <string>

struct Identifier {
	std::string Tag;

	explicit Identifier(const char *tag)
		: Tag(tag) {
	}
};