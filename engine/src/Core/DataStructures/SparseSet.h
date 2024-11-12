#pragma once
#include <iostream>
#include <vector>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include <utility>


class SparseSet {
public:
	explicit SparseSet(size_t maxElement);

	void Add(size_t element);
	void Remove(size_t element);
	bool Contains(size_t element);
	void Resize(size_t maxElement);

public:
	using iterator = typename std::vector<size_t>::iterator;
	using const_iterator = typename std::vector<size_t>::const_iterator;

	iterator begin();
	iterator end();

	[[nodiscard]] const_iterator begin() const;
	[[nodiscard]] const_iterator end() const;

	[[nodiscard]] const_iterator cbegin() const;
	[[nodiscard]] const_iterator cend() const;

private:
	bool Contains(size_t element, size_t &packedIndex);

protected:
	std::vector<size_t>	sparseArray;
	std::vector<size_t>	packedArray;
};
