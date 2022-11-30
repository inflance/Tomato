#include "Layer.h"

namespace Tomato {

	Layer::Layer(const std::string& name /*= "Layer"*/)
		:m_debug_name(name)
	{
		LogSystem::ConsoleLog(name, LogType::Trace);
	}
	Layer::~Layer()
	{

	}

}