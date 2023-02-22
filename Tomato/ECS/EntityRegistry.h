#pragma once
#include <entt/entt.hpp>

namespace Tomato
{
	class Entity;

	class EntityRegistry
	{
	public:
		EntityRegistry() = default;
		EntityRegistry(std::string name);

		Entity CreateEntity();
	
		void DestoryEntity(Entity& entity);

		auto& Get() { return m_registry; }
	private:
		std::string m_name;
		entt::registry m_registry;
		friend class Entity;
		friend class Scene;
	};
	
}
