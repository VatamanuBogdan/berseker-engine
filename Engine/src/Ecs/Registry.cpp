#include "Registry.h"

namespace Ecs {

	Entity Registry::CreateEntity() {
		Entity entity(entitiesIdCounter++);
		return std::move(entity);
	}

}
