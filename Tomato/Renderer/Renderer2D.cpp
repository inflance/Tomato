#include "Tomato/tmtpch.h"

#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"
#include "Tomato/Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <Tomato/Scene/Entity.h>


namespace Tomato {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 Texcoord;

		float TexIndex;
		//平铺因子
		float TilingFactor;

		int EntityID;
	};

	struct QuadData {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;

		static const uint32_t MaxTextureSlot = 32;

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<Shader> TextureShader;
		std::shared_ptr<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferArr = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlot> TextureSlots;

		uint32_t TextureSlotIndex = 0;
		//for vertex position
		glm::vec4 QuadVertexPosition[4];

		Tomato::Renderer2D::Statistics Stats;
	};

	static QuadData s_quad_data;

	void Renderer2D::Init()
	{

		s_quad_data.QuadVertexArray = VertexArray::Create();
		s_quad_data.QuadVertexBuffer = VertexBuffer::Create(s_quad_data.MaxVertices * sizeof(QuadVertex));

		//设置顶点布局
		s_quad_data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3		, "a_Position"		},
			{ShaderDataType::Float4		, "a_Color"			},
			{ShaderDataType::Float2		, "a_TexCoord"		},
			{ShaderDataType::Float		, "a_TexIndex"		},
			{ShaderDataType::Float		, "a_TilingFactor"	},
			{ShaderDataType::Int		, "a_EntityID"		},
		});
		s_quad_data.QuadVertexArray->AddVertexBuffer(s_quad_data.QuadVertexBuffer);

		s_quad_data.QuadVertexBufferArr = new QuadVertex[s_quad_data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_quad_data.MaxIndices];
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_quad_data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}																

		std::shared_ptr<IndexBuffer> squareIndexBuffer = IndexBuffer::Create(quadIndices, s_quad_data.MaxIndices );
		s_quad_data.QuadVertexArray->SetIndexBuffer(squareIndexBuffer);
		delete[] quadIndices;
		
		//white
		s_quad_data.WhiteTexture = Texture2D::Create(1,1);
		uint32_t whiteTextureData = 0xffffffff;
		s_quad_data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int samplers[s_quad_data.MaxTextureSlot];
		for (uint32_t i = 0; i < s_quad_data.MaxTextureSlot; i++)
			samplers[i] = i;

		s_quad_data.TextureShader = Shader::Create("PreCompile/Assets/Shader/texture.glsl");
		s_quad_data.TextureShader->Bind();
		s_quad_data.TextureShader->SetIntArray("u_Textures", samplers, s_quad_data.MaxTextureSlot);

		//init vertex position
		s_quad_data.QuadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f}; //left buttom 
		s_quad_data.QuadVertexPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f};  //left top
		s_quad_data.QuadVertexPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f}; //right top
		s_quad_data.QuadVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f}; //right buttom

		s_quad_data.TextureSlots[0] = s_quad_data.WhiteTexture;

	}

	void Renderer2D::Shutdown()
	{
		delete[] s_quad_data.QuadVertexBufferArr;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_quad_data.TextureShader->Bind();
		s_quad_data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMat());
		s_quad_data.QuadIndexCount = 0;
		s_quad_data.QuadVertexBufferPtr = s_quad_data.QuadVertexBufferArr;

		s_quad_data.TextureSlotIndex = 1;
	
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{

		glm::mat4 ViewProjection = camera.GetProjection() * glm::inverse(transform);

		s_quad_data.TextureShader->Bind();
		s_quad_data.TextureShader->SetMat4("u_ViewProjection", ViewProjection);
		s_quad_data.QuadIndexCount = 0;
		s_quad_data.QuadVertexBufferPtr = s_quad_data.QuadVertexBufferArr;

		s_quad_data.TextureSlotIndex = 1;

	}


	void Renderer2D::BeginScene(const EditorCamera& camera)
	{

		glm::mat4 ViewProjection = camera.GetViewProjection();

		s_quad_data.TextureShader->Bind();
		s_quad_data.TextureShader->SetMat4("u_ViewProjection", ViewProjection);
		s_quad_data.QuadIndexCount = 0;
		s_quad_data.QuadVertexBufferPtr = s_quad_data.QuadVertexBufferArr;

		s_quad_data.TextureSlotIndex = 1;
	}

	void Renderer2D::StartNewBatch()
	{
		s_quad_data.QuadIndexCount = 0;
		s_quad_data.QuadVertexBufferPtr = s_quad_data.QuadVertexBufferArr;

		s_quad_data.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (s_quad_data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_quad_data.QuadVertexBufferPtr - (uint8_t*)s_quad_data.QuadVertexBufferArr);
			s_quad_data.QuadVertexBuffer->SetData(s_quad_data.QuadVertexBufferArr, dataSize);
			//bind texture

			for (auto i = 0; i < s_quad_data.TextureSlotIndex; i++)
			{
				s_quad_data.TextureSlots[i]->BindUnit(i);
			}
			s_quad_data.TextureShader->Bind();
			RendererCommand::DrawIndexed(s_quad_data.QuadVertexArray, s_quad_data.QuadIndexCount);

#ifdef DEBUG
			++s_quad_data.Stats.DrawCalls;
#endif // DEBUG
		}
		

	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartNewBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int GID)
	{
		// out 
		if (s_quad_data.QuadIndexCount >= QuadData::MaxIndices)
		{
			NextBatch();
		}

		constexpr glm::vec2 texcoord[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;
		constexpr uint32_t QuadVertexCount = 4;

		for (int i = 0; i < QuadVertexCount; i++)
		{
			s_quad_data.QuadVertexBufferPtr->Position = transform * s_quad_data.QuadVertexPosition[i];
			s_quad_data.QuadVertexBufferPtr->Color = color;
			s_quad_data.QuadVertexBufferPtr->Texcoord = texcoord[i];
			s_quad_data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_quad_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_quad_data.QuadVertexBufferPtr->EntityID = GID;
			s_quad_data.QuadVertexBufferPtr++;
		}

		s_quad_data.QuadIndexCount += 6;


#ifdef DEBUG
		++s_quad_data.Stats.QuadCount;
#endif // DEBUG
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D> texture, const float tilingFactor, const glm::vec4& color, int GID)
	{
		glm::vec2 texcoord[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		float textureIndex = 0.0f;
		constexpr uint32_t QuadVertexCount = 4;

		for (uint32_t i = 1; i < s_quad_data.TextureSlotIndex; i++)
		{
			if (*s_quad_data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_quad_data.TextureSlotIndex >= s_quad_data.MaxTextureSlot)
				NextBatch();

			textureIndex = (float)s_quad_data.TextureSlotIndex;
			s_quad_data.TextureSlots[s_quad_data.TextureSlotIndex] = texture;
			s_quad_data.TextureSlotIndex++;
		}

		for (int i = 0; i < QuadVertexCount; i++)
		{
			s_quad_data.QuadVertexBufferPtr->Position = transform * s_quad_data.QuadVertexPosition[i];
			s_quad_data.QuadVertexBufferPtr->Color = color;
			s_quad_data.QuadVertexBufferPtr->Texcoord = texcoord[i];
			s_quad_data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_quad_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_quad_data.QuadVertexBufferPtr->EntityID = GID;
			s_quad_data.QuadVertexBufferPtr++;
		}

		s_quad_data.QuadIndexCount += 6;

#ifdef DEBUG
		++s_quad_data.Stats.QuadCount;
#endif // DEBUG
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const std::shared_ptr<SubTexture2D> subTexture, const float tilingFactor, const glm::vec4& color,int GID)
	{

		if (s_quad_data.QuadIndexCount >= QuadData::MaxIndices)
		{
			StartNewBatch();
		}

		std::shared_ptr<Texture2D> texture = subTexture->GetTexture();

		glm::vec2* texcoord = subTexture->GetTextcoord();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_quad_data.TextureSlotIndex; i++)
		{
			if (*s_quad_data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_quad_data.TextureSlotIndex;
			s_quad_data.TextureSlots[s_quad_data.TextureSlotIndex] = texture;
			s_quad_data.TextureSlotIndex++;
		}

		constexpr uint32_t QuadVertexCount = 4;
		for (int i = 0; i < QuadVertexCount; i++)
		{
			s_quad_data.QuadVertexBufferPtr->Position = transform * s_quad_data.QuadVertexPosition[i];
			s_quad_data.QuadVertexBufferPtr->Color = color;
			s_quad_data.QuadVertexBufferPtr->Texcoord = texcoord[i];
			s_quad_data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_quad_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_quad_data.QuadVertexBufferPtr->EntityID = GID;
			s_quad_data.QuadVertexBufferPtr++;
		}

		s_quad_data.QuadIndexCount += 6;


#ifdef DEBUG
		++s_quad_data.Stats.QuadCount;
#endif // DEBUG
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SpriteComponent& cc, int GID )
	{
		if (cc.Texture)
			DrawQuad(transform, cc.Texture, cc.TilingFactor, cc.Color, GID);
		else
			DrawQuad(transform, cc.Color, GID);
	}

#ifdef DEBUG
	Tomato::Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_quad_data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_quad_data.Stats, 0, sizeof(Statistics));
	}

#endif // DEBUG

	
	

}

