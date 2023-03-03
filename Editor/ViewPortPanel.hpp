#pragma once

#include "Tomato/Core/Structs.h"

namespace Tomato
{
	class ViewPortPanel
	{
	public:
		ViewPortPanel(void* id, float width, float height, std::string name = "ViewPort");
		~ViewPortPanel();

		void Begin() const;
		void Tick();
		void End() const;

		void UpdateViewPort(float width, float height);

		void UpdateName(std::string name);

		void UpdateImage(void* id);

		[[nodiscard]] Vec2 GetViewPortSize() const { return Vec2{ m_width, m_height }; }

		using EventFn = std::function<void(float, float)>;

		void SetEventTick(EventFn&& fn);

		static std::shared_ptr<ViewPortPanel> Create(void* id, float width, float height, std::string name = "ViewPort");
	private:
		void* m_id;
		float m_width, m_height;
		std::string m_name;
		std::vector<EventFn> m_fn;
	};
}

