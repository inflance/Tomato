#pragma once

#include "Tomato/ECS/Entity.h"

class b2World;

namespace Tomato
{
	class Level
	{
	public:
		Level(std::string level_name = "Default Level");

		Entity CreateDefaultEntity(const std::string& name);

		void OnRuntimeLevelStart();

		void OnRuntimeLevelEnd();

		void Tick(float delta_time) const;

		void TickOnEditor(const Ref<EditorCamera>& camera, float delta_time) const;

		void LogicTick(float delta_time) const;

		[[nodiscard]] const auto& GetLevelName() const { return m_name; }

		void SetLevelName(const std::string& name) { m_name = name; }

		[[nodiscard]] const auto& GetLevelRegistry() const { return m_registry; }

		void OnViewPortResize(uint32_t width, uint32_t height);

	private:
		uint32_t m_width{}, m_height{};
		std::string m_name;
		Ref<EntityRegistry> m_registry;
		b2World* m_physical_world = nullptr;
	};
}
