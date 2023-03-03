#include "Level.hpp"

#include "Tomato/Renderer/Renderer2D.hpp"

#include "box2d/box2d.h"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_shape.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

#include "Tomato/ECS/Components.h"
#include "Tomato/ECS/EntityRegistry.h"
#include "Tomato/Function/Camera/EditorCamera.hpp"

inline static std::unordered_map<Tomato::RigidBody2DComponent::BodyType, b2BodyType> body_type_map = {
	{Tomato::RigidBody2DComponent::BodyType::Static, b2_staticBody},
	{Tomato::RigidBody2DComponent::BodyType::Dynamic, b2_dynamicBody},
	{Tomato::RigidBody2DComponent::BodyType::Kinematic, b2_kinematicBody},
};

namespace Tomato
{
	Level::Level(std::string level_name)
		: m_name(std::move(level_name))
	{
		m_registry = std::make_shared<EntityRegistry>(m_name);
	}

	Entity Level::CreateDefaultEntity(const std::string& name)

	{
		auto entity = m_registry->CreateEntity();
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Level::OnRuntimeLevelStart()
	{
		m_physical_world = new b2World({0.0f, -9.8f});

		const auto view = m_registry->Get().view<RigidBody2DComponent>();
		for (const auto e : view)
		{
			Entity entity = {e, m_registry.get()};
			const auto& transform = entity.GetComponent<TransformComponent>();

			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = body_type_map[rb2d.type_];
			//if vulkan
			bodyDef.position.Set(transform.position_.x, -transform.position_.y);
			bodyDef.angle = -transform.rotation_.z;

			b2Body* body = m_physical_world->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.fixed_rotation_);
			rb2d.body_ = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				const auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.size_.x * transform.scale_.x, bc2d.size_.y * transform.scale_.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.density_;
				fixtureDef.friction = bc2d.friction_;
				fixtureDef.restitution = bc2d.restitution_;
				fixtureDef.restitutionThreshold = bc2d.restitution_threshold_;
				body->CreateFixture(&fixtureDef);
			}
			/*
			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}*/
		}
	}

	void Level::OnRuntimeLevelEnd()
	{
		const auto view = m_registry->Get().view<RigidBody2DComponent, ResetComponent<TransformComponent>>();
		for (const auto e : view)
		{
			Entity entity = {e, m_registry.get()};
			auto& ts = entity.GetComponent<ResetComponent<TransformComponent>>();
			auto& transform = entity.GetComponent<TransformComponent>();
			transform.position_ = ts.component_.position_;
			transform.rotation_ = ts.component_.rotation_;
			transform.scale_ = ts.component_.scale_;
		}
		delete m_physical_world;
		m_physical_world = nullptr;
	}

	void Level::Tick(float delta_time) const
	{
		std::optional<Camera> main_camera{};
		std::optional<glm::mat4> transform{};
		{
			const auto view = m_registry->Get().view<CameraComponent, TransformComponent>();

			for (const auto entity : view)
			{
				auto& tc = view.get<TransformComponent>(entity);
				auto& cc = view.get<CameraComponent>(entity);
				if (cc.is_main_camera_)
				{
					main_camera = cc.camera_;
					transform = tc.GetTransform();
				}
			}
		}

		//start renderer
		if (main_camera.has_value() && transform.has_value())
		{
			Renderer2D::Begin(main_camera.value(), transform.value());
			{
				const auto& view = m_registry->Get().view<LightComponent, TransformComponent>();

				for (auto& e : view)
				{
					Entity entity = {e, m_registry.get()};

					auto& tc = view.get<TransformComponent>(e);

					auto& lc = view.get<LightComponent>(e);

					lc.light_.position = tc.position_;

					Renderer2D::RenderLight(lc.light_);
				}
			}

			{
				const auto& view = m_registry->Get().view<TransformComponent, SpriteComponent>();

				for (auto& entity : view)
				{
					auto& tc = view.get<TransformComponent>(entity);
					auto& sp = view.get<SpriteComponent>(entity);
					if (sp.geometry_type_ == SpriteComponent::Render2DType::Quad)
					{
						Renderer2D::RenderQuad(tc.GetTransform(), sp.color_);
					}
					else if (sp.geometry_type_ == SpriteComponent::Render2DType::Circle)
					{
						Renderer2D::RenderCircle(tc.GetTransform(), sp.color_);
					}
				}
			}
			Renderer2D::End();
		}
	}

	void Level::TickOnEditor(const Ref<EditorCamera>& camera, float delta_time) const
	{
		Renderer2D::Begin(camera);
		{
			const auto& view = m_registry->Get().view<NativeScriptComponent>();

			for (const auto entity : view)
			{
				auto& tc = view.get<NativeScriptComponent>(entity);
				tc.Bind<ScriptableEntity>();
				tc.instance_->Tick(delta_time);
			}
		}

		{
			const auto& view = m_registry->Get().view<
				LightComponent, TransformComponent, ResetComponent<TransformComponent>>();

			for (auto& e : view)
			{
				Entity entity = {e, m_registry.get()};

				auto& tc = view.get<TransformComponent>(e);
				auto& lc = view.get<LightComponent>(e);
				lc.light_.position = tc.position_;
				lc.light_.direction_ = toMat4(glm::quat({ -tc.rotation_.x , -tc.rotation_.y, tc.rotation_.z})) * glm::vec4{ 0.0f, 0.0f, -1.0f, 1.0f };
				Renderer2D::RenderLight(lc.light_);
			}
		}
		{
			const auto& view = m_registry->Get().view<TransformComponent, SpriteComponent>();

			for (auto& e : view)
			{
				Entity entity = {e, m_registry.get()};

				auto& tc = view.get<TransformComponent>(e);

				auto& sp = view.get<SpriteComponent>(e);

				if (sp.geometry_type_ == SpriteComponent::Render2DType::Quad)
				{
					Renderer2D::RenderQuad(tc.GetTransform(), sp.color_);
				}
				else if (sp.geometry_type_ == SpriteComponent::Render2DType::Circle)
				{
					Renderer2D::RenderCircle(tc.GetTransform(), sp.color_);
				}
			}
		}
		Renderer2D::End();

		{
			const auto& view = m_registry->Get().view<TransformComponent>();

			{
				for (auto& e : view)
				{
					Entity entity = {e, m_registry.get()};

					auto& tc = view.get<TransformComponent>(e);
					// save the reset value
					if (!entity.HasComponent<ResetComponent<TransformComponent>>())
						entity.AddComponent<ResetComponent<TransformComponent>>(tc);
					else
						entity.GetComponent<ResetComponent<TransformComponent>>().component_ = tc;
				}
			}
		}
	}

	void Level::LogicTick(float delta_time) const
	{
		// Physics
		{
			constexpr int32_t velocityIterations = 6;
			constexpr int32_t positionIterations = 2;
			m_physical_world->Step(delta_time, velocityIterations, positionIterations);

			// Retrieve transform from Box2D
			const auto view = m_registry->Get().view<RigidBody2DComponent>();
			for (const auto e : view)
			{
				Entity entity = {e, m_registry.get()};
				auto& transform = entity.GetComponent<TransformComponent>();
				const auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

				const b2Body* body = static_cast<b2Body*>(rb2d.body_);
				const auto& position = body->GetPosition();
				transform.position_.x = position.x;
				//if vulkan
				transform.position_.y = -position.y;
				transform.rotation_.z = -body->GetAngle();
			}
		}
	}

	void Level::OnViewPortResize(float width, float height)
	{
		m_width = width;
		m_height = height;
		{
			const auto view = m_registry->Get().view<CameraComponent>();

			for (const auto entity : view)
			{
				auto& cc = view.get<CameraComponent>(entity);
				if (cc.is_main_camera_)
				{
					cc.camera_.SetAspectRatio(m_width / m_height);
					cc.camera_.UpdateProjectionMatrix();
				}
			}
		}
	}
}
