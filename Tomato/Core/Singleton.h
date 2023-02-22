#pragma once

namespace Tomato
{
	template <class T>
	class Singleton
	{
	public:
		static T& Get()
		{
			static T instance;
			return instance;
		}

		virtual ~Singleton() noexcept = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

	protected:
		Singleton() = default;
	};
}
