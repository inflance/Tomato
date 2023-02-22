#pragma once

#include "Tomato/ECS/Entity.h"
#include "Tomato/ECS/EntityRegistry.h"

namespace Tomato
{
	class Level
	{
	public:
		Level(std::string level_name = "Default Value");

		[[nodiscard]] Entity CreateDefaultEntity(std::string name);

		void Tick(float delta_time);
		void TickOnEditor(float delta_time);
		void LogicTick(float delta_time);

		const auto& GetLevelName() const { return name; }

	private:
		std::string name;
		Ref<EntityRegistry> m_registry;
	};
}
