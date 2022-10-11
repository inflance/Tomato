#pragma once

#include <glfw/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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

		void BlockEvents(bool block) { m_blockEvent = block; }

	private:
		bool m_blockEvent = true;
	};
}