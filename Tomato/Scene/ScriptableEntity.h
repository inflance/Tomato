#pragma once

#include "Entity.h"
#include "Scene.h"

namespace Tomato{

	class ScriptableEntity
	{
	public:
		ScriptableEntity() = default;
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}
		
	protected:
		virtual void OnCreate() {};
		virtual void Tick(float deltaTime) {
			std::cout << "123" << std::endl;
		};
		virtual void OnDestroy(){};

	private:
		Entity m_entity;

		friend class Scene;
	};
}