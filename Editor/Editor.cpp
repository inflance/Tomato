#include "Editor.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Tomato/Math.h"
#include "Tomato/Timer.h"
#include <imgui_demo.cpp>
#include "Tomato/Scene/ScriptableGO.h"
#include "Tomato/IFile.h"

#include "Tomato/KeyCode.h"
#include <ImGuizmo.h>

namespace Tomato{

    Editor::Editor()
        :Layer("Editor"), m_cameraControler(1600.0f / 800.0f)
    {

    }

    void Editor::OnAttach()
    {
        m_texture = Texture2D::Create("C:/Users/liyunlo2000/source/repos/Tomato/Tomato/Precompile/Image/DefaultTexture.png");
        m_texture1 = Texture2D::Create("C:/Users/liyunlo2000/source/repos/Tomato/Tomato/Precompile/Image/tilemap_packed.png");
        m_subtexture = SubTexture2D::CreateSubtexture(m_texture1, { 0.0f, 0.0f }, { 73.0f, 73.0f }, { 1.0f, 0.71f });

        FrameBufferProps fb_props;
        fb_props.Height = 800;
        fb_props.Width = 1600;

        m_frameBuffer = IFrameBuffer::Create(fb_props);

        m_Scene = std::make_shared<Scene>();

#if 0
         squareObject = m_Scene->CreateGameObject("square");
         squareObject.AddComponent<ColorComponent>();

         squareObject1 = m_Scene->CreateGameObject("square1");
         squareObject1.AddComponent<ColorComponent>();

         CameraA = m_Scene->CreateGameObject("CameraA");
         CameraA.AddComponent<CameraComponent>();


         CameraB = m_Scene->CreateGameObject("CameraB");
         CameraB.AddComponent<CameraComponent>();

         auto& cc = CameraB.GetComponent<CameraComponent>().IsMain = false;
#endif
        class CameraControllor : public ScriptableGO
        {
            void Tick(TimeSpan ts) override
            {
                auto& position = GetComponent<TransformComponent>().Position;

                float speed = 5.0f;

                if (Input::IsKeyPressed(KEY_A))
                    position.x -= speed * ts;
                if (Input::IsKeyPressed(KEY_D))
                    position.x += speed * ts;
                if (Input::IsKeyPressed(KEY_W))
                    position.y += speed * ts;
                if (Input::IsKeyPressed(KEY_S))
                    position.y -= speed * ts;
            }
        };

        /* CameraA.AddComponent<NativeScriptComponent>().Bind<CameraControllor>();
         CameraB.AddComponent<NativeScriptComponent>().Bind<CameraControllor>();*/

        SceneSerializater m_SceneSerializater(m_Scene);
        if (m_SceneSerializater.DeSerialization("D:/redcube.json"))
        {
            //m_SceneSerializater.Serialization("D:/2.json");
        }
       
        m_ScenePanel.SetContex(m_Scene);
    }

    void Editor::OnDetach()
    {

    }

    void Editor::Tick(TimeSpan ts)
    {

        /*if (m_viewPortFocused)
            m_cameraControler.Tick(ts);*/

        Renderer2D::ResetStats();
        //m_Scene->Tick(ts);
        m_frameBuffer->Bind();
        RendererCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        RendererCommand::Clear();

        m_Scene->Tick(ts);

        m_frameBuffer->UnBind();

    }

    void Editor::OnImGuiRender()
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

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
               /* ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);*/

                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    CreateNewScene();
                }

                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    OpenScene();
                }

                if (ImGui::MenuItem("Save as", "Ctrl+Shift+S"))
                {
                    SaveSceneAs();
                }
                    
#if 0                    
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
#endif
                ImGui::Separator();

                if (ImGui::MenuItem("Close", NULL, false, open != NULL))
                    TomatoEngine::GetInstance().Close();
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
            ImGui::ShowDemoWindow(&open);
        }
        if (showViewPort)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("ViewPort");


            //set event block
            {
                m_viewPortFocused = ImGui::IsWindowFocused();
                m_viewPortHovered = ImGui::IsWindowHovered();

                if (!m_viewPortFocused && !m_viewPortHovered)
                {
                    TomatoEngine::GetInstance().GetImGuiLayer()->SetBlock(true);
                }
                else {
                    TomatoEngine::GetInstance().GetImGuiLayer()->SetBlock(false);
                }
            }

            //set window resize
            {
                ImVec2 viewPortSize = ImGui::GetContentRegionAvail();

                if (m_viewPortSize != *(glm::vec2*)&viewPortSize && viewPortSize.x > 0.0f && viewPortSize.y > 0.0f)
                {
                    m_viewPortSize = { viewPortSize.x, viewPortSize.y };
                    LOG_WARN("{0}. {1}", m_viewPortSize.x, m_viewPortSize.y);


                    m_frameBuffer->Resize(m_viewPortSize.x, m_viewPortSize.y);
                    m_cameraControler.Resize(m_viewPortSize.x, m_viewPortSize.y);
                    m_Scene->SetViewPortResize(m_viewPortSize.x, m_viewPortSize.y);
                }

            }

            

            uint32_t id = m_frameBuffer->GetColorAttach();

            ImGui::Image((void*)id, ImVec2{ m_viewPortSize.x, m_viewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


            {
                GameObject SelectedGO = m_ScenePanel.GetSelectedGO();

                if (SelectedGO && (m_editorMode != -1))
                {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();

                    float width = (float)ImGui::GetWindowWidth();
                    float height = (float)ImGui::GetWindowHeight();

                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

                    auto& cameraGO = m_Scene->GetMainCameraGO();

                    const auto& cc = cameraGO.GetComponent<CameraComponent>().Camera;
                    auto cameraProjection = cc.GetProjection();
                    auto cameraView = glm::inverse(cameraGO.GetComponent<TransformComponent>().GetTransform());

                    auto& tc = SelectedGO.GetComponent<TransformComponent>();
                    auto transform = SelectedGO.GetComponent<TransformComponent>().GetTransform();

                    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_editorMode, ImGuizmo::LOCAL, glm::value_ptr(transform));

                    if (ImGuizmo::IsUsing())
                    {
                        glm::vec3 position, rotation, scale;

                        Math::DecomposeTransform(transform, position, rotation, scale);

                        glm::vec3 deltaRotation = rotation - tc.Rotation;

                        tc.Position = position;
                        tc.Rotation += deltaRotation;
                        tc.Scale = scale;
                    }

                }
            }
            

            ImGui::End();
            ImGui::PopStyleVar();
        }
        {
            ImGui::Begin("setting");
      
            auto& stats = Renderer2D::GetStats();
            ImGui::Text("DrawCalls:%d ", stats.DrawCalls);
            ImGui::Text("Quad: %d", stats.QuadCount);
            ImGui::Text("TotalIndex: %d", stats.GetTotalIndexCount());
            ImGui::Text("TotalVertex: %d", stats.GetTotalVetexCount());


            ImGui::Text("fps: %u", TomatoEngine::GetInstance().GetFPS());


            ImGui::Text("123");
            m_ScenePanel.OnImGuiRenderer();
            ImGui::End();
        }

        ImGui::End();
    }

    void Editor::OnEvent(Event& event)
    {
        m_cameraControler.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(&Editor::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(&Editor::OnMouseButtonPressed));
    }

	void Editor::CreateNewScene()
	{
        m_Scene = std::make_shared<Scene>();
        m_Scene->SetViewPortResize(m_viewPortSize.x , m_viewPortSize.y);
        m_ScenePanel.SetContex(m_Scene);
	}

	void Editor::OpenScene()
	{
        const std::string& filePath = Tomato::File::FileDiolog::OpenFile("Tomato Scene (*.json)\0*.json\0");
        if (!filePath.empty())
        {
            std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

            SceneSerializater m_SceneSerializater(newScene);
            if (m_SceneSerializater.DeSerialization(filePath))
            {
                m_Scene = newScene;
                m_Scene->SetViewPortResize(m_viewPortSize.x, m_viewPortSize.y);
                m_ScenePanel.SetContex(m_Scene);
            }
        }
	}

	void Editor::SaveSceneAs()
	{
        const std::string& filePath = Tomato::File::FileDiolog::SaveFile("Tomato Scene (*.json)\0*.json\0");
        if (!filePath.empty())
        {
            SceneSerializater m_SceneSerializater(m_Scene);
            if (m_SceneSerializater.Serialization(filePath))
            {
               

            }
        }
	}

	bool Editor::OnKeyPressed(KeyPressedEvent& e)
	{
        LOG_INFO(e.ToString());

        bool ctrl = Input::IsKeyPressed(KEY_LEFT_CONTROL) || Input::IsKeyPressed(KEY_RIGHT_CONTROL);
        bool alt = Input::IsKeyPressed(KEY_LEFT_ALT) || Input::IsKeyPressed(KEY_RIGHT_ALT);
        bool shift = Input::IsKeyPressed(KEY_LEFT_SHIFT) || Input::IsKeyPressed(KEY_RIGHT_SHIFT);

        switch (e.getKeyCode())
        {
        
        case KEY_N:
            if (ctrl)
            {
                CreateNewScene();
                
            }
            break;
        case KEY_O:
            if (ctrl) {
                OpenScene();
                
            }
            break;
        case KEY_S:       
            if (shift && ctrl) {
                SaveSceneAs();
              
            }
            break;
        case KEY_Q:
                m_editorMode = -1;
            break;
        case KEY_W:
                m_editorMode = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case KEY_E:

                m_editorMode = ImGuizmo::OPERATION::ROTATE;

            break;
        case KEY_R:
                m_editorMode = ImGuizmo::OPERATION::SCALE;

            break;

        }

        return false;
	}

	bool Editor::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
        return false;
	}

}

