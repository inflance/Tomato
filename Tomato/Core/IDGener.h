#pragma once

namespace Tomato
{
	class IDGenerator
	{
	public:
		IDGenerator()
			: m_cur_id(0)
		{
		}

		IDGenerator(size_t id)
			: m_cur_id(id)
		{
		}

		auto GenID()
		{
			return m_cur_id++;
		}

	private:
		std::atomic<size_t> m_cur_id;
	};
}
