#include "Entity.h"


namespace Ecs {

Entity::Entity(Entity &&entity) noexcept
	: id(entity.id), mask(entity.mask) {
	entity.id = -1;
	entity.mask.reset();
}

}