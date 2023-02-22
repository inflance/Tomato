#pragma once

#include "Level.h"
#include "Tomato/Core/Singleton.h"
#include "Tomato/Core/Singleton.h"

namespace Tomato
{
	class World 
	{
	public:
		World(std::string name = "World");

		void AddLevel(const Level& level);

		auto GetLevel(const std::string& name);

		void LogicTick(float delta_time);

		void RenderTick(float delta_time);

		void EditorTick(float delta_time);


	private:
		std::string m_name{};
		std::map<std::string, Level> m_levels;
	};
}
