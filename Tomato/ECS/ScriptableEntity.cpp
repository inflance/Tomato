#include "ScriptableEntity.hpp"

namespace Tomato
{
	ScriptableEntity::ScriptableEntity(entt::entity handle, EntityRegistry* registry)
		:Entity(handle, registry)
	{
	}

	ScriptableEntity::ScriptableEntity(const Entity& entity)
		:Entity(entity)
	{
	}
}
