#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "Tomato/Renderer/Renderer.h"
#include "Tomato/Renderer/Renderer2D.h"
#include "Tomato/Renderer/ShaderFactory.h"

namespace Tomato {

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<NameComponent>();
		tag.Name = name.empty() ? ("Empty Entity"+std::to_string((uint32_t)entity)) : name;

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

	Tomato::Entity Scene::CreateStaticMesh(const std::string& name /*= std::string()*/)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<StaticMeshComponent>();
		auto& tag = entity.AddComponent<NameComponent>();
		tag.Name = name.empty() ? "StaticMesh" : name;

		entity.GetComponent<StaticMeshComponent>().StaticMesh.Load(uint32_t(entity));

		return entity;
	}

	Tomato::Entity Scene::CreateBaseShape(const std::string& name /*= std::string()*/)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<BaseShapeComponent>();
		entity.AddComponent<SpriteComponent>();
		entity.AddComponent<MatirialComponent>();
		MatirialTextureData diffuse;
		MatirialTextureData specular;
		diffuse.Texture= Texture2D::Create(std::string("PreCompile/Assets/Image/container2.png"));
		specular.Texture = Texture2D::Create(std::string("PreCompile/Assets/Image/container2_specular.png"));
		entity.GetComponent<MatirialComponent>().matiral.SetDiffuseMap(diffuse);
		entity.GetComponent<MatirialComponent>().matiral.SetSpecularMap(specular);
		auto& tag = entity.AddComponent<NameComponent>();
		tag.Name = name.empty() ? "Cube" : name;

		entity.GetComponent<BaseShapeComponent>().BaseShape.CreateCube(uint32_t(entity));

		return entity;
	}

	Tomato::Entity Scene::CreateLight(const std::string& name /*= std::string()*/)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<LightComponent>();
		entity.AddComponent<BaseShapeComponent>();
		auto& tag = entity.AddComponent<NameComponent>();
		tag.Name = name.empty() ? "Light" : name;

		entity.GetComponent<BaseShapeComponent>().BaseShape.CreateCube(uint32_t(entity));

		return entity;
	}

	void Scene::TickEditor(float deltaTime, const EditorCamera& camera)
	{
		{
			Renderer2D::BeginScene(camera);
			const auto& view = m_Registry.view<TransformComponent, SpriteComponent>();

			for (auto& entity : view)
			{
				auto& transformComponent = view.get<TransformComponent>(entity);
				auto& spriteComponent = view.get<SpriteComponent>(entity);

				Renderer2D::DrawQuad(transformComponent.GetTransform(), spriteComponent, (int)entity);

			}
			Renderer2D::EndScene();
		}


		const auto& shader = ShaderFactory::GetInstance().GetShader("BaseCube");
		shader->Bind();
		shader->SetFloat3("u_CameraPos", camera.GetPosition());
		{
			const auto& view = m_Registry.view<TransformComponent, StaticMeshComponent>();

			for (auto& entity : view)
			{
				auto& transformComponent = view.get<TransformComponent>(entity);
				auto& staticMeshComponent = view.get<StaticMeshComponent>(entity);

				Renderer::RenderBaseShape(staticMeshComponent.StaticMesh, camera.GetViewProjection(), transformComponent.GetTransform());

			}
		}

		{

			const auto& view = m_Registry.view<TransformComponent, BaseShapeComponent, SpriteComponent, MatirialComponent>();
			for (auto& entity : view)
			{
				auto& transformComponent = view.get<TransformComponent>(entity);
				auto& baseShapeComponent = view.get<BaseShapeComponent>(entity);
				auto& spriteComponent = view.get<SpriteComponent>(entity);
				auto& matirialComponent = view.get<MatirialComponent>(entity);

				Renderer::RenderBaseShapeWithMatirial(baseShapeComponent.BaseShape, spriteComponent.Color, matirialComponent.matiral, camera.GetViewProjection(), transformComponent.GetTransform());

			}
		}

		{
			const auto& lcv = m_Registry.view<LightComponent>();
			int pointLightSize = 0;
			for (auto& entity : lcv)
			{
				auto& lightComponent = lcv.get<LightComponent>(entity);
				if(lightComponent.Light.GetLightType() == LightType::PointLight)
					pointLightSize++;
			}

			const auto& view = m_Registry.view<TransformComponent, BaseShapeComponent, LightComponent>();
			for (auto& entity : view)
			{
				auto& transformComponent = view.get<TransformComponent>(entity);
				auto& baseShapeComponent = view.get<BaseShapeComponent>(entity);
				auto& lightComponent = view.get<LightComponent>(entity);

				Renderer::RenderBaseLight(baseShapeComponent.BaseShape, lightComponent.Light, camera.GetViewProjection(), transformComponent.GetTransform(), transformComponent.Position, pointLightSize);

			}
		}
		
		
	}

	void Scene::Tick(float deltaTime)
	{

		{
			const auto& view = m_Registry.view<NativeScriptComponent>();

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
			const auto& view = m_Registry.view<TransformComponent, CameraComponent>();
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
			const auto& view = m_Registry.view<TransformComponent, SpriteComponent>();
	
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
		const auto& view = m_Registry.view<CameraComponent>();
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