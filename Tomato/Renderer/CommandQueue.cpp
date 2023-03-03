#include "CommandQueue.hpp"

namespace Tomato
{
	CommandQueue::CommandQueue()
	{
	}

	CommandQueue::~CommandQueue()
	{
	}

	void CommandQueue::Allocate(CommandFn&& func)
	{
		m_command_queue.emplace(std::move(func));
	}

	void CommandQueue::Execute()
	{
		while (!m_command_queue.empty())
		{
			const auto& func = m_command_queue.front();
			func();
			m_command_queue.pop();
		}
	}
}
