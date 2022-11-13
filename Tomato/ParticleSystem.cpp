#include "tmtpch.h"
#include "ParticleSystem.h"

#include "Renderer/Renderer2D.h"
#include "Random.h"
#include "Core.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Tomato
{

	ParticleSystem::ParticleSystem(uint32_t size)
		:m_pool_index(size-1)
	{
		m_particle_pool.resize(size);
	}

	void ParticleSystem::Tick(TimeSpan ts)
	{
		for (auto& particle : m_particle_pool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				particle.LifeRemaining = 0.0f;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;
		}
	}

	void ParticleSystem::OnRender(OrthographicCamera& camera)
	{

			Renderer2D::BeginScene(camera);
			for (auto& particle : m_particle_pool)
			{
				if (!particle.Active)
					continue;

				// Fade away particles
				float life = particle.LifeRemaining / particle.LifeTime;
				glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
				//color.a = color.a * life;

				float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
				glm::vec3 position = { particle.Position.x, particle.Position.y, 0.5f };
				Tomato::Renderer2D::DrawQuad(position, glm::radians(particle.Rotation), { size, size }, color);
			}
			Renderer2D::EndScene();
	}

	void ParticleSystem::EmitParticle(const ParticleProps& particleProps)
	{
		Particle& particle = m_particle_pool[m_pool_index];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = Random::GetFloat() * 2.0f * glm::pi<float>();

		//LOG_INFO(Random::GetFloat());

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::GetFloat() - 0.5f);
		//LOG_INFO(Random::GetFloat());
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::GetFloat() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::GetFloat() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		m_pool_index = --m_pool_index % m_particle_pool.size();
	}

}