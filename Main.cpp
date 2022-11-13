#pragma once
//----Tomato Engine header file----//
#include "Tomato/Tomato.h"
//----Tomato Engine header file----//

//----2D Engine Client----//
#include "Editor/Editor.h"
//----2D Engine Client----//
// 
//----entry point----//
int main(int argc, char** argv)
{
	Tomato::LogSystem::Init();
	Tomato::TomatoEngine::GetInstance().StartUp();
	//start 2d layer//
	Tomato::TomatoEngine::GetInstance().PushLayer(new Tomato::Editor());
	Tomato::TomatoEngine::GetInstance().Run();
	Tomato::TomatoEngine::GetInstance().ShutDown();

	return 0;
}
//----entry point----//