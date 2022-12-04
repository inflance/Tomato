#pragma once

namespace Tomato {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);

		~UUID();

		operator uint64_t()
		{
			return m_uuid;
		}

		operator const uint64_t() const
		{
			return m_uuid;
		}
	private:
		uint64_t m_uuid;
	};

}

namespace std {

	template <>
	struct hash<Tomato::UUID>
	{
		std::size_t operator()(const Tomato::UUID& uuid) const
		{
			return uuid;
		}
	};
}
