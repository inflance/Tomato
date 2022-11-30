#pragma once

#define DEBUG

#include "OrthographicCamera.h"

#include "Tomato/Scene/Components.h"
#include "Tomato/Function/Camera/Camera.h"
#include "Tomato/Function/Camera/EditorCamera.h"
#include "Tomato/Renderer/Texture.h"
#include "Tomato/Renderer/SubTexture2D.h"

namespace Tomato {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform );
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Flush();

		static void StartNewBatch();
		static void NextBatch();
		
		static void DrawQuad(const glm::mat4& transform, const SpriteComponent& cc, int GID = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f), int GID = -1);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f), int GID = -1);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<SubTexture2D> subtexture, const float tilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f), int GID = -1);

#ifdef DEBUG
		//
		struct Statistics
		{
			//��Ⱦ���õ�����
			uint32_t DrawCalls = 0;
			//���θ���
			uint32_t QuadCount = 0;

			//��������
			uint32_t GetTotalVetexCount() { return QuadCount * 4; }
			//��������
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();
#endif
	};
}
