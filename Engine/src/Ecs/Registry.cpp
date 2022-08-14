#include "Registry.h"

#include <utility>

namespace Ecs {

Registry::~Registry() {
	// TODO
}

Entity Registry::CreateEntity() {
	Entity entity(entitiesIdCounter++);
	return std::move(entity);
}

}
