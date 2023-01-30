#include "CommandQueue.h"

namespace Tomato
{
	
	CommandQueue::CommandQueue()
	{
		m_command_queue.resize(512 * 1024);
	}

	CommandQueue::~CommandQueue()
	{
		m_command_queue.clear();
	}

	void CommandQueue::Allocate(CommandFn&& func)
	{
		m_command_queue.emplace_back(std::forward<CommandFn>(func));
	}

	void CommandQueue::Execute()
	{
		for(auto& func : m_command_queue)
		{
			func();
		}
		m_command_queue.clear();
	}

}