#pragma once

#include "GameObject.h"
#include "Scene.h"
#include "Tomato/TimeSpan.h"

namespace Tomato{

	class ScriptableGO
	{
	public:
		ScriptableGO() = default;
		virtual ~ScriptableGO() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_GO.GetComponent<T>();
		}
		
	protected:
		virtual void OnCreate() {};
		virtual void Tick(TimeSpan ts) {
			std::cout << "123" << std::endl;
		};
		virtual void OnDestroy(){};

	private:
		GameObject m_GO;

		friend class Scene;
	};
}