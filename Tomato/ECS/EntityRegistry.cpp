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
		const auto entity = m_registry.create();
		return { entity, this };
	}

	void EntityRegistry::DestroyEntity(const Entity& entity)
	{
		m_registry.destroy(entity);
	}
}
