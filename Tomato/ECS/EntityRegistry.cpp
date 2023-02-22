#include "EntityRegistry.h"

#include "Entity.h"

namespace Tomato
{
	EntityRegistry::EntityRegistry(std::string name)
		:m_name(std::move(name))
	{
	}

	Entity EntityRegistry::CreateEntity()
	{
		auto entity = m_registry.create();
		return Entity(entity, this);
	}

	void EntityRegistry::DestoryEntity(Entity& entity)
	{
		m_registry.destroy(entity);
	}
}
