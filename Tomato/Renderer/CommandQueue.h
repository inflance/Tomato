#pragma once

namespace Tomato
{
	class CommandQueue
	{
	public:
		using CommandFn = std::function<void(void)>;

		CommandQueue();
		~CommandQueue();

		void Allocate(CommandFn&& func);

		void Execute();

	private:
		std::vector<CommandFn> m_command_queue;
	};
}
