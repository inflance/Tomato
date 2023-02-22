#include "Level.h"

#include "Tomato/ECS/Components.h"

namespace Tomato
{
	Level::Level(std::string level_name)
	{
		m_registry = std::make_shared<EntityRegistry>(std::move(level_name));
	}

	Entity Level::CreateDefaultEntity(std::string name)
	{
		auto entity = m_registry->CreateEntity();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Level::Tick(float delta_time)
	{

		const auto& view = m_registry->Get().view<TagComponent>();

		for (auto& entity : view)
		{
			auto& tc = view.get<TagComponent>(entity);
			LOG_INFO(tc.tag_);
		}
	}

	void Level::TickOnEditor(float delta_time)
	{
	}

	void Level::LogicTick(float delta_time)
	{
	}
}
