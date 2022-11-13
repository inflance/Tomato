#include "GameObject.h"

namespace Tomato{


	GameObject::GameObject(entt::entity handle, Scene* scene)
		:m_ObjectHandle(handle), m_Scene(scene)
	{

	}

	void GameObject::Clear()
	{
		m_ObjectHandle = entt::null;
		m_Scene = nullptr;
	}

}