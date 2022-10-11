#pragma once

#include "Tomato/Tomato.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Layer2D.h"
 
using namespace std;

int main(int argc, char** argv)
{
	Tomato::LogSystem::Init();
	//LOG_ASSERT(true, "Unknown ShaderDataType!");
	Tomato::TomatoEngine::GetInstance().StartUp();
	Tomato::TomatoEngine::GetInstance().PushLayer(new Layer2D());
	Tomato::TomatoEngine::GetInstance().Run();
	Tomato::TomatoEngine::GetInstance().ShutDown();
	return 0;
}
