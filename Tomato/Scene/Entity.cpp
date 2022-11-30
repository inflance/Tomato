#include "Entity.h"

namespace Tomato{


	Entity::Entity(entt::entity handle, Scene* scene)
		:m_ObjectHandle(handle), m_Scene(scene){}

	void Entity::Clear()
	{
		m_ObjectHandle = entt::null;
		m_Scene = nullptr;
	}

}