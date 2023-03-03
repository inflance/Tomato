#include "TestLayer.h"
#include "Tomato/Core/Core.hpp"
#include "Editor/Editor.h"
#include "Tomato/Renderer/Vulkan/Test.h"
#include "Tomato/Renderer/Buffer.hpp"

int main(int argc, char** argv)
{
	/*std::thread thread([]
		{*/
	Tomato::Log::Init();
	Tomato::Engine::Get().StartUp();

	//Tomato::Test();
	Tomato::Engine::Get().PushLayer(new Tomato::TestLayer());
	Tomato::Engine::Get().Run();
	Tomato::Engine::Get().ShutDown();
	//	});
	//thread.join();
	return 0;
}
