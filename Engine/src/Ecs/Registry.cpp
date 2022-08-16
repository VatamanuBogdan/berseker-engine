#include "Registry.h"

#include <utility>

namespace Ecs {

Registry::~Registry() {
	for (size_t i = 0; i < componentSetDeleters.size(); i++) {
		auto deleter = componentSetDeleters[i];
		if (deleter) {
			deleter(componentSets[i]);
		}
	}
}

Entity Registry::CreateEntity() {
	Entity entity(entitiesIdCounter++);
	return std::move(entity);
}

}
