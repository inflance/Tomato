#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "Tomato/Renderer/Renderer2D.h"

namespace Tomato {

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<NameComponent>();
		tag.Name = name.empty() ? ("Empty Entity"+std::to_string(entity)) : name;

		return entity;
	}

	Tomato::Entity Scene::CreateSprite(const std::string& name /*= std::string()*/)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<SpriteComponent>();
		auto& tag = entity.AddComponent<NameComponent>();
		tag.Name = name.empty() ? "Sprite" : name;

		return entity;
	}

	Tomato::Entity Scene::CreateCamera(const std::string& name /*= std::string()*/)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<CameraComponent>();
		auto& tag = entity.AddComponent<NameComponent>();
		tag.Name = name.empty() ? "Camera" : name;
		
		return entity;
	}

	void Scene::TickEditor(float deltaTime, const EditorCamera& camera)
	{

		Renderer2D::BeginScene(camera);
		auto& view = m_Registry.view<TransformComponent, SpriteComponent>();

		for (auto& entity : view)
		{
			auto& transformComponent = view.get<TransformComponent>(entity);
			auto& spriteComponent = view.get<SpriteComponent>(entity);

			Renderer2D::DrawQuad(transformComponent.GetTransform(), spriteComponent, (int)entity);

		}
		Renderer2D::EndScene();
	}

	void Scene::Tick(float deltaTime)
	{

		{
			auto& view = m_Registry.view<NativeScriptComponent>();

			for (auto& entity : view)
			{
				auto& nsc = view.get<NativeScriptComponent>(entity);
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->Tick(deltaTime);
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
			auto& view = m_Registry.view<TransformComponent, SpriteComponent>();
	
			for (auto entity : view)
			{
				auto& transform = view.get<TransformComponent>(entity);
				auto& color = view.get<SpriteComponent>(entity);
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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::Clear()
	{
		m_Registry.each([&](entt::entity entity) {
			LOG_ERROR((int)entity);
			m_Registry.destroy(entity);
			});
	}

	Tomato::Entity Scene::GetMainCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto en : view)
		{
			auto& camera = view.get<CameraComponent>(en);
			if (camera.IsMain)
			{
				return Entity{ en, this };
			}
		}
		return {};
	}

}