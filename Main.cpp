#pragma once

#include "Editor/Editor.h"

int main(int argc, char** argv)
{
	Tomato::LogSystem::Init();
	Tomato::TomatoEngine::GetInstance().StartUp();
	Tomato::TomatoEngine::GetInstance().PushLayer(new Tomato::Editor());
	//”Œœ∑—≠ª∑
	Tomato::TomatoEngine::GetInstance().Run();
	Tomato::TomatoEngine::GetInstance().ShutDown();

	return 0;
}
