#pragma once
#include <functional>
#include <cstdint>

#ifdef NDEBUG
#define OnDebug(expression)
#else
#define OnDebug(expression) expression
#endif

#ifdef NDEBUG
#define DebugClassMember(type, member)
#else
#define DebugClassMember(type, member) type member;
#endif

#define SafeNullableCall(object, property) \
	if (object) {                          \
		((object)->property);			\
	}

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
	void Init(Args... args) {
		new (memory) T(args...);
	}

	T& operator*() { return *reinterpret_cast<T*>(memory); }
	const T& operator*() const { return *reinterpret_cast<T*>(memory); }

	T* operator->() { return reinterpret_cast<T*>(memory); }
	const T* operator->() const { return reinterpret_cast<T*>(memory); }

private:
	uint8_t memory[sizeof(T)]{};
};