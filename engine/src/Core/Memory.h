#pragma once
#include <cstdint>
#include <memory>
#include <cstring>


#define SafeNullableCall(object, property) 	\
	if (object) {					\
		((object)->property);			\
	}

template <typename T>
class ObsRef {
public:
	explicit ObsRef(T *reference = nullptr) {
		this->reference = reference;
	}

	ObsRef(const ObsRef<T> &obsRef) {
		this->reference = obsRef->reference;
	}

	void Set(T *reference) {
		this->reference = reference;
	}

	void Invalidate() {
		this->reference = nullptr;
	}

	explicit operator bool () {
		return reference != nullptr;
	}

	bool operator==(T *reference) {
		return this->reference == reference;
	}

	bool operator==(const ObsRef<T> &obsRef) {
		return this->reference == obsRef.reference;
	}

	ObsRef& operator=(T *reference) {
		this->reference = reference;
	}

	ObsRef& operator=(const ObsRef<T> &obsRef) {
		this->reference = obsRef.reference;
	}

	T& operator*() { return *reference; }
	const T& operator*() const { return *reference; }

	T* operator->() { return reference; }
	const T* operator->() const { return reference; }

private:
	T *reference = nullptr;
};

template <typename T>
class Lateinit final {
public:
	Lateinit() = default;

	Lateinit(const Lateinit<T> &copied) {
		this->operator=(copied);
	}

	Lateinit(Lateinit<T> &&moved) noexcept {
		this->operator=(std::move(moved));
	}

	~Lateinit() {
		this->operator*().~T();
	}

	Lateinit<T>& operator=(const Lateinit<T> &copied) {
		if (this != &copied) {
			new (memory) T(*copied);
		}
		return *this;
	}

	Lateinit<T>& operator=(Lateinit<T> &&moved) noexcept {
		if (moved != &moved) {
			new(memory) T(std::move(*moved));
		}
		return *this;
	}

	template <typename ...Args>
	void Init(Args&&... args) {
		new (memory) T(std::forward<Args>(args)...);
	}

	T& operator*() { return *reinterpret_cast<T*>(memory); }
	const T& operator*() const { return *reinterpret_cast<T*>(memory); }

	T* operator->() { return reinterpret_cast<T*>(memory); }
	const T* operator->() const { return reinterpret_cast<T*>(memory); }

private:
	uint8_t memory[sizeof(T)]{};
};

template <typename SwapType>
void MemorySwap(SwapType &var1, SwapType &var2) {
	constexpr size_t size = sizeof(SwapType);
	uint8_t temp[size];

	memcpy(static_cast<void*>(temp), static_cast<void*>(&var1), size);
	memcpy(static_cast<void*>(&var1), static_cast<void*>(&var2), size);
	memcpy(static_cast<void*>(&var2), temp, size);
}