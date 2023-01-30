#pragma once

#include <entt/entt.hpp>

#include "Scene.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Core/UUID.h"

namespace Tomato
{
	class Entity
	{
	public:
		Entity() = default;

		virtual ~Entity()
		{
		};

		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		void SetEntityID(UUID uuid) { m_EntityID = uuid; }
		const UUID GetEntityID() const { return m_EntityID; };

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			LOG_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_ObjectHandle, std::forward<Args>(args)...);
			//m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template <typename T>
		T& GetComponent()
		{
			LOG_ASSERT(HasComponent<T>(), "Object does not have component!");
			return m_Scene->m_Registry.get<T>(m_ObjectHandle);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_ObjectHandle);
		}

		template <typename T>
		void RemoveComponent()
		{
			LOG_ASSERT(HasComponent<T>(), "Object does not have component!");
			m_Scene->m_Registry.remove<T>(m_ObjectHandle);
		}

		operator bool() const { return m_ObjectHandle != entt::null; }
		operator entt::entity() const { return m_ObjectHandle; }
		operator uint32_t() const { return static_cast<uint32_t>(m_ObjectHandle); }


		bool operator==(const Entity& other) const
		{
			return m_ObjectHandle == other.m_ObjectHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		void Clear();

	private:
		UUID m_EntityID;

	protected:
		entt::entity m_ObjectHandle{entt::null};
		Scene* m_Scene = nullptr;
	};
}
