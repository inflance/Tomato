#include "World.h"

#include <ranges>

namespace Tomato
{
	World::World(std::string name)
		:m_name(std::move(name))
	{
	}

	void World::AddLevel(const Level& level)
	{
		const auto& name = level.GetLevelName();
		if(m_levels.contains(name))
		{
			LOG_ERROR("Level {} is already exists", name);
			return;
		}
		m_levels[name] = level;
	}

	auto World::GetLevel(const std::string& name)
	{
		if (!m_levels.contains(name))
		{
			LOG_ERROR("Level {} is not in world", name);
			return Level{};
		}
		return m_levels[name];
	}

	void World::LogicTick(float delta_time)
	{
		for(auto& level : m_levels | std::views::values)
		{
			level.LogicTick(delta_time);
		}
	}

	void World::RenderTick(float delta_time)
	{
		for (auto& level : m_levels | std::views::values)
		{
			level.Tick(delta_time);
		}
	}

	void World::EditorTick(float delta_time)
	{
		for (auto& level : m_levels| std::views::values)
		{
			level.TickOnEditor(delta_time);
		}
	}
}
