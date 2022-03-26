#pragma once
#include <functional>

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