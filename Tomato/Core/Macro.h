#pragma once

#include "Log.h"

#define BIT(x) (1 << (x))
//log macro
#define  LOG_TRACE(...) ::Tomato::Log::Trace( __VA_ARGS__ )
#define  LOG_INFO(...) ::Tomato::Log::Info( __VA_ARGS__ )
#define  LOG_WARN(...) ::Tomato::Log::Warn( __VA_ARGS__ )
#define  LOG_ERROR(...) ::Tomato::Log::Error( __VA_ARGS__ )
#define  LOG_ASSERT(x, ...) if(!(x)){LOG_ERROR(__VA_ARGS__); __debugbreak();}
#define  ASSERT(x) assert(x)

//event bind func
#define BIND_EVENT_FUNC(...) std::bind(&__VA_ARGS__, this, std::placeholders::_1)

#define NAME_OF(name) #name

#define NEW new(__FILE__, __LINE__)

template <typename T>
std::string NameOfType()
{
	return typeid(T).name();
}

#include <memory>

namespace Tomato
{
	template <typename T>
	using UPtr = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename... TArgs>
	constexpr auto CreateRef(TArgs&&... args)
	{
		return std::make_shared<T>(std::forward<TArgs>(args)...);
	}

	template <typename T, typename Ptr>
	constexpr std::shared_ptr<T> As(const Ptr& ptr)
	{
		return std::dynamic_pointer_cast<T>(ptr);
	}
}
