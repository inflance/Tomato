#pragma once

#include "Entity.h"

namespace Tomato
{
	class ScriptableEntity : public Entity
	{
	public:
		ScriptableEntity() = default;
		ScriptableEntity(entt::entity handle, EntityRegistry* registry);
		ScriptableEntity(const Entity& entity);

		virtual ~ScriptableEntity() = default;

		virtual void OnCreate() {}
		virtual void Tick(float delta_time)
		{
			LOG_TRACE(delta_time);
		}
		virtual void OnDestroy(){}
	private:
		friend class Level;
	};
}