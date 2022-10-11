#pragma once

#include "OrthographicCamera.h"
#include "Tomato/Renderer/Texture.h"

#define DEBUG

namespace Tomato {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void StartNewBatch();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));


		static void DrawQuad(const glm::vec2& position, const float angle, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float angle, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const float angle, const glm::vec2& size, const std::shared_ptr<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const float angle, const glm::vec2& size, const std::shared_ptr<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
	
#ifdef DEBUG
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVetexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();
#endif
	};
}