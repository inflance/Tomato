#pragma once

namespace Tomato
{
	template<class T>
	class PublicSingleton
	{
	public:
		static T& Get() noexcept (std::is_nothrow_constructible_v<T>)
		{
			static T instance;
			return instance;
		}
		virtual ~PublicSingleton() noexcept = default;
		PublicSingleton(const PublicSingleton&) = delete;
		PublicSingleton& operator=(const PublicSingleton&) = delete;

	protected:
		PublicSingleton() = default;
	};
}