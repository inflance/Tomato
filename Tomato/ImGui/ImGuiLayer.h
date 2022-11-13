#pragma once

#include "Tomato/Engine.h"
#include "Tomato/LogSystem.h"
#include "Tomato/Layer.h"

namespace Tomato {

	class ImGuiLayer : public Layer 
	{
	public:

		ImGuiLayer()
			: Layer("ImGuiLayer")
		{
		}
		~ImGuiLayer(){};

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void SetBlock(bool block) { m_block = block; }

	private:
		bool m_block = true;
	};
}