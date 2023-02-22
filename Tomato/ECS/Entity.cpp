#include "Entity.h"

namespace Tomato
{
	Entity::Entity(entt::entity handle, EntityRegistry* registry)
		:m_object_handle(handle), m_registry(registry)
	{
	}
}
