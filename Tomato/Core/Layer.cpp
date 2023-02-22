#include "Layer.h"

namespace Tomato
{
	Layer::Layer(const std::string& name /*= "Layer"*/)
		: m_debug_name(name)
	{
		LOG_TRACE(m_debug_name + " Created");
	}

	Layer::~Layer()
	{
		LOG_TRACE(m_debug_name + " Destroyed");
	}
}
