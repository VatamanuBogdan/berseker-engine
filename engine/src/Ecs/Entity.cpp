#include "Entity.h"


namespace Ecs {

Entity::Entity(Entity &&entity) noexcept
	: id(entity.id) {
	entity.id = -1;
}

Entity::Entity(IdType id)
	: id(id) {
}

}