#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "Tomato/Core/Core.h"

namespace Tomato {

	OpenGLRendererAPI::OpenGLRendererAPI()
	{

	}

	void OpenGLRendererAPI::Init()
	{
		//启用混合
		glEnable(GL_BLEND);
		//启用混合函数
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//开启深度测试
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		//glEnable(GL_MULTISAMPLE);

	}

	void OpenGLRendererAPI::SetViewPort(int x, int y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount() ;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

	}

}