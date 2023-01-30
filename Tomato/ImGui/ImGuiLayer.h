#pragma once

#include "Tomato/Core/Layer.h"


namespace Tomato
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer()
			: Layer("ImGuiLayer")
		{
		}

		~ImGuiLayer() override = default;

		void OnCreate() override;
		void OnDestroy() override;
		void OnEvent(Event& event) override;
		void OnImGuiRenderer() override;

		virtual void Begin();
		virtual void End();

		virtual void SetDarkModeColor();

		virtual void SetBlock(bool block) { m_block = block; }

		static ImGuiLayer* Create();

	protected:
		bool m_block = true;
	};
}
