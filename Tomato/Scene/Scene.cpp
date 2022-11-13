#include "Tomato/tmtpch.h"

#include "Scene.h"
#include "Components.h"
#include "GameObject.h"
#include "Tomato/Renderer/Renderer2D.h"

namespace Tomato {

	GameObject Scene::CreateGameObject(const std::string& name)
	{
		GameObject GO = { m_Registry.create(), this };
		GO.AddComponent<TransformComponent>();
		auto& tag = GO.AddComponent<NameComponent>();
		tag.Name = name.empty() ? "GameObject" : name;

		//m_GameObjects.push_back(GO);
		return GO;
	}

	void Scene::Tick(TimeSpan ts)
	{

		{
			auto& view = m_Registry.view<NativeScriptComponent>();

			for (auto& entity : view)
			{
				auto& nsc = view.get<NativeScriptComponent>(entity);
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_GO = GameObject{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->Tick(ts);
			}
		}



		Camera* mainCamera = nullptr;
		glm::mat4 mainCameraTransform;

		{
			auto& view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& transform = view.get<TransformComponent>(entity);
				auto& camera = view.get<CameraComponent>(entity);

				if (camera.IsMain) {
					mainCamera = &camera.Camera;
					mainCameraTransform = transform.GetTransform();
					break;
				}
			}

		}

		if (mainCamera) 
		{
			Renderer2D::BeginScene(*mainCamera, mainCameraTransform);
			auto& view = m_Registry.view<TransformComponent, ColorComponent>();
	
			for (auto entity : view)
			{
				auto& transform = view.get<TransformComponent>(entity);
				auto& color = view.get<ColorComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), color.Color);
			}
			Renderer2D::EndScene();
		}

	}

	void Scene::SetViewPortResize(float width, float height)
	{
		m_viewPortWidth = width;
		m_viewPortHeight = height;
		auto& view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);

			if (!camera.IsResize)
			{
				camera.Camera.SetViewPortSize(m_viewPortWidth, m_viewPortHeight);
			}
		}
	}

	void Scene::DestroyEntity(GameObject GO)
	{
		m_Registry.destroy(GO);
	}

	Tomato::GameObject Scene::GetMainCameraGO()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto en : view)
		{
			auto& camera = view.get<CameraComponent>(en);
			if (camera.IsMain)
			{
				return GameObject{ en, this };
			}
		}
		return {};
	}

}