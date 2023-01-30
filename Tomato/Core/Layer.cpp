#include "Layer.h"

namespace Tomato
{
	Layer::Layer(const std::string& name /*= "Layer"*/)
		: m_debug_name(name)
	{
		LogSystem::Log(LogType::Trace, m_debug_name + " Created");
	}

	Layer::~Layer()
	{
		LogSystem::Log(LogType::Trace, m_debug_name + " Destroyed");
	}
}
