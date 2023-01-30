#pragma once
#include "Tomato/Core/Core.h"
#include "Editor/Editor.h"
#include "Tomato/Renderer/Vulkan/Test.h"

int main(int argc, char** argv)
{
	Tomato::LogSystem::Init();
	Tomato::TomatoEngine::Get().StartUp();

	Tomato::TomatoEngine::Get().PushLayer(new Tomato::Editor());
	Tomato::TomatoEngine::Get().Run();
	Tomato::TomatoEngine::Get().ShutDown();

	return 0;
}
