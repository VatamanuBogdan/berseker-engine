#include "SparseSet.h"

SparseSet::SparseSet(size_t maxElement) {
	Resize(maxElement);
}

void SparseSet::Add(size_t element) {
	if (Contains(element)) {
		return;
	}

	if (sparseArray.size() <= element) {
		Resize(element);
	}

	sparseArray[element] = packedArray.size();
	packedArray.push_back(element);
}

void SparseSet::Remove(size_t element) {
	size_t targetPackedIndex;
	if (!Contains(element, targetPackedIndex)) {
		return;
	}

	size_t lastPackedIndex = packedArray.size();
	std::swap(sparseArray[targetPackedIndex], sparseArray[lastPackedIndex]);

	packedArray[targetPackedIndex] = packedArray[lastPackedIndex];
	packedArray.pop_back();
}

bool SparseSet::Contains(size_t element) {
	size_t packedIndex;
	if (!Contains(element, packedIndex)) {
		return false;
	}
	return packedIndex == element;
}

void SparseSet::Resize(size_t maxElement) {
	sparseArray.resize(maxElement + 1);
}

bool SparseSet::Contains(size_t element, size_t &packedIndex) {
	if (sparseArray.size() <= element) {
		return false;
	}

	size_t sparseIndex = sparseArray[element];
	if (packedArray.size() <= sparseIndex) {
		return false;
	}

	packedIndex = packedArray[sparseIndex];
	return packedIndex == element;
}

SparseSet::iterator SparseSet::begin() {
	return packedArray.begin();
}

SparseSet::iterator SparseSet::end() {
	return packedArray.end();
}

SparseSet::const_iterator SparseSet::begin() const {
	return packedArray.begin();
}

SparseSet::const_iterator SparseSet::end() const {
	return packedArray.end();
}

SparseSet::const_iterator SparseSet::cbegin() const {
	return packedArray.cbegin();
}

SparseSet::const_iterator SparseSet::cend() const {
	return packedArray.cend();
}
