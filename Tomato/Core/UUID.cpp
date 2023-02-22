#include "UUID.h"

#include <random>

namespace Tomato {

	static std::random_device s_rd;
	static std::mt19937_64 s_engine(s_rd());
	static std::uniform_int_distribution<uint64_t> s_uid;

	UUID::UUID()
		:m_uuid(s_uid(s_engine))
	{

	}

	UUID::UUID(uint64_t uuid)
		:m_uuid(uuid)
	{

	}

	UUID::~UUID()
	{

	}

}