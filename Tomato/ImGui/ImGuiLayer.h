#pragma once

#include "Tomato/Core/Layer.h"

namespace Tomato {

	class ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer()
			: Layer("ImGuiLayer"){}
		~ImGuiLayer() = default;

		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRenderer() override;
		
		void Begin();
		void End();
		
		void SetDarkModeColor();

		void SetBlock(bool block) { m_block = block; }

	private:
		bool m_block = true;
	};
}