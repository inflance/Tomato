#pragma once
#include "Tomato/ECS/Entity.h"

namespace Tomato
{
	struct TransformComponent;
	class Entity;
	class Level;

	class LevelPanel final
	{
	public:
		LevelPanel() = default;
		LevelPanel(const std::shared_ptr<Level>& context);

		void ResetContext(const std::shared_ptr<Level>& context) { m_level_context = context; }

		~LevelPanel();

		void Tick();

		void DrawLevelPanel(Entity entity);

		auto GetSelectedEntity()const { return m_selected_entity; }
		void DrewDetailPanel(const Entity& entity);

		template<typename TComponent>
		void DrawMenu(const std::string& name, Entity entity);

		template <typename TComponent, typename Func>
		void DrawComponents(const std::string& name, Entity Entity, Func uiFunction);
	private:
		bool m_is_open = true;
		Entity m_selected_entity{};
		std::shared_ptr<Level> m_level_context{};
	};

}
