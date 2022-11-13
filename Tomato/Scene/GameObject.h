#pragma once

#include "Tomato/Core.h"
#include "Scene.h"

#include <entt/entt.hpp>

namespace Tomato {

	class GameObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject() {};

		GameObject(entt::entity handle, Scene* scene);
		GameObject(const GameObject& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			LOG_ASSERT(!HasComponent<T>(), "Object already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_ObjectHandle, std::forward<Args>(args)...);
			//m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			LOG_ASSERT(HasComponent<T>(), "Object does not have component!");
			return m_Scene->m_Registry.get<T>(m_ObjectHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_ObjectHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			LOG_ASSERT(HasComponent<T>(), "Object does not have component!");
			m_Scene->m_Registry.remove<T>(m_ObjectHandle);
		}

		operator bool() const { return m_ObjectHandle != entt::null; }
		operator entt::entity() const { return m_ObjectHandle; }
		operator uint32_t() const { return (uint32_t)m_ObjectHandle; }


		bool operator==(const GameObject& other) const
		{
			return m_ObjectHandle == other.m_ObjectHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const GameObject& other) const
		{
			return !(*this == other);
		}

		void Clear();

	private:
		entt::entity m_ObjectHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};

}