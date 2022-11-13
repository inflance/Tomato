#include "Layer2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Tomato/Timer.h"
#include <imgui_demo.cpp>

Layer2D::Layer2D()
	:Layer("Layer2D"), m_cameraControler(1600.0f / 800.0f)
{

}

void Layer2D::OnAttach()
{
	m_texture = Tomato::Texture2D::Create("C:/Users/liyunlo2000/source/repos/Tomato/Tomato/Precompile/Image/DefaultTexture.png");
	m_texture1 = Tomato::Texture2D::Create("C:/Users/liyunlo2000/source/repos/Tomato/Tomato/Precompile/Image/tilemap_packed.png");
	m_subtexture = Tomato::SubTexture2D::CreateSubtexture(m_texture1, { 0.0f, 0.0f }, { 73.0f, 73.0f }, { 1.0f, 0.71f });
	//m_square_shader = Tomato::Shader::Create("D:/Dev/cmake/Tomato/Tomato/precompile/square.vert", "D:/Dev/cmake/Tomato/Tomato/precompile/square.frag");
	// Init here
	/*m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 2.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };*/

    Tomato::FrameBufferProps fb_props;
    fb_props.Height = 800;
    fb_props.Width = 1600;

    m_frameBuffer = Tomato::IFrameBuffer::Create(fb_props);
}

void Layer2D::OnDetach()
{

}

void Layer2D::Tick(Tomato::TimeSpan ts)
{

	//Tomato::Timer timer("layer2D::Tick");
	
    if(m_viewPortFocused)
	    m_cameraControler.Tick(ts);

	Tomato::Renderer2D::ResetStats();

	static float rotation = 0;
	rotation += float(ts) * 50.0f ;
    m_frameBuffer->Bind();
	Tomato::RendererCommand::SetClearColor(glm::vec4(0.1f, 0.6f, 0.5f, 1.0f));
	Tomato::RendererCommand::Clear();

	Tomato::Renderer2D::BeginScene(m_cameraControler.GetCamera());
    Tomato::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_texture, 10.0f);
    for (float i = -5.0f; i <= 5.0f; i += 0.5f)
    {
        for (float j = -5.0f; j <= 5.0f; j += 0.5f)
        {
            glm::vec4 color = { (i + 5.0f) / 10.0f,(j + 5.0f) / 10.0f , (j + i + 10.0f) / 20.0f, 0.7f };
            Tomato::Renderer2D::DrawQuad({ i, j, 0.0f }, { 0.45f, 0.45f }, color);
        }
    }
	Tomato::Renderer2D::DrawQuad({ 0.3f, 0.0f },glm::radians( rotation), { 1.0f, 1.0f }, { 0.12f, 0.0f, 0.2f, 1.0f });
	Tomato::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.12f, 0.0f, 0.2f, 1.0f });
	Tomato::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 1.0f }, { 0.8f, 0.0f, 0.2f, 1.0f });
	
	Tomato::Renderer2D::DrawQuad({ -0.5f, 0.0f, 0.1f }, glm::radians( - 45.0f), {0.5f, 0.5f}, m_texture, 1.0f);
	Tomato::Renderer2D::EndScene();

	

	/*if (Tomato::Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		glm::vec2 mousePos = Tomato::Input::GetMousePosition();
		auto width = Tomato::TomatoEngine::GetInstance().GetWindow().GetWidth();
		auto height = Tomato::TomatoEngine::GetInstance().GetWindow().GetHeight();

		auto bounds = m_cameraControler.GetBounds();
		auto pos = m_cameraControler.GetCamera().GetPosition();
		mousePos.x = (mousePos.x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		mousePos.y = bounds.GetHeight() * 0.5f - (mousePos.y / height) * bounds.GetHeight();
		m_particle.Position = { mousePos.x + pos.x, mousePos.y + pos.y};
		for (int i = 0; i < 20; i++)
			m_particleSystem.EmitParticle(m_particle);
	}

	m_particleSystem.Tick(ts);
	m_particleSystem.OnRender(m_cameraControler.GetCamera());*/

	Tomato::Renderer2D::BeginScene(m_cameraControler.GetCamera());
	//Tomato::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_texture1);
	Tomato::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 0.7f }, m_subtexture);
	Tomato::Renderer2D::EndScene();

    m_frameBuffer->UnBind();

}

void Layer2D::OnImGuiRender()
{

    bool open = true;

    static bool showViewPort = true;

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handled the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        ShowDockingDisabledMessage();
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, open != NULL))
                Tomato::TomatoEngine::GetInstance().Close();
            ImGui::EndMenu();
        }
        HelpMarker(
            "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n"
            "- Drag from window title bar or their tab to dock/undock." "\n"
            "- Drag from window menu button (upper-left button) to undock an entire node (all windows)." "\n"
            "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == false, default)" "\n"
            "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)" "\n"
            "This demo app has nothing to do with enabling docking!" "\n\n"
            "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window." "\n\n"
            "Read comments in ShowExampleAppDockSpace() for more details.");

        ImGui::EndMenuBar();
      
    }
    if(showViewPort)
    {   ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("ViewPort");
        ImGui::SetItemDefaultFocus();

        m_viewPortFocused = ImGui::IsWindowFocused();
        m_viewPortHovered = ImGui::IsWindowHovered();

        if (m_viewPortFocused && m_viewPortHovered)
        {
            Tomato::TomatoEngine::GetInstance().GetImGuiLayer()->SetBlock(false);
        }
        else {
            Tomato::TomatoEngine::GetInstance().GetImGuiLayer()->SetBlock(true);
        }
        ImVec2 viewPortSize = ImGui::GetContentRegionAvail();

        if (m_viewPortSize != *(glm::vec2*)&viewPortSize && viewPortSize.x > 0.0f && viewPortSize.y > 0.0f)
        {
            m_viewPortSize = { viewPortSize.x, viewPortSize.y };
            LOG_WARN("{0}. {1}", m_viewPortSize.x, m_viewPortSize.y);

            m_frameBuffer->Resize(m_viewPortSize.x, m_viewPortSize.y);
            m_cameraControler.Resize(m_viewPortSize.x, m_viewPortSize.y);
        }

        uint32_t id = m_frameBuffer->GetColorAttach();
        ImGui::Image((void*)id, ImVec2{ m_viewPortSize.x, m_viewPortSize.y }, ImVec2{0, 1}, ImVec2{1, 0});
        ImGui::End();
        ImGui::PopStyleVar();
    }
    {
        ImGui::Begin("setting");
        ImGui::ColorEdit4("triangle color", glm::value_ptr(m_color));

        auto& stats = Tomato::Renderer2D::GetStats();
        ImGui::Text("DrawCalls:%d ", stats.DrawCalls);
        ImGui::Text("Quad: %d", stats.QuadCount);
        ImGui::Text("TotalIndex: %d", stats.GetTotalIndexCount());
        ImGui::Text("TotalVertex: %d", stats.GetTotalVetexCount());


        ImGui::Text("fps: %u", Tomato::TomatoEngine::GetInstance().GetFPS());


        ImGui::Text("123");
        ImGui::End();
    }
    
    ImGui::End();
}

void Layer2D::OnEvent(Tomato::Event& event)
{
	m_cameraControler.OnEvent(event);
}

