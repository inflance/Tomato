#pragma once

#include <entt/entt.hpp>

#include "EntityRegistry.h"
#include "Tomato/Core/Core.hpp"
#include "Tomato/Scene/Scene.hpp"

namespace Tomato
{
	class Entity
	{
	public:
		Entity() = default;

		virtual ~Entity() = default;

		Entity(entt::entity handle, EntityRegistry* registry);
		Entity(const Entity& other) = default;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>()) { LOG_ERROR("Entity already has component!"); return GetComponent<T>(); };
			T& component = m_registry->m_registry.emplace<T>(m_object_handle, std::forward<Args>(args)...);
			//m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template <typename T>
		T& GetComponent()
		{
			LOG_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_registry->m_registry.get<T>(m_object_handle);
		}

		template <typename T>
		[[nodiscard]]bool HasComponent()const
		{
			return m_registry->m_registry.all_of<T>(m_object_handle);
		}

		template <typename T>
		void RemoveComponent() const
		{
			LOG_ASSERT(HasComponent<T>(), "Object does not have component!");
			m_registry->m_registry.remove<T>(m_object_handle);
		}

		operator bool() const { return m_object_handle != entt::null; }

		operator entt::entity() const { return m_object_handle; }

		operator uint32_t() const { return static_cast<uint32_t>(m_object_handle); }

		bool operator==(const Entity& other) const
		{
			return m_object_handle == other.m_object_handle && m_registry == other.m_registry;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	protected:
		entt::entity m_object_handle{entt::null};
		EntityRegistry* m_registry{};

		friend class EntityRegistry;
	};
}
