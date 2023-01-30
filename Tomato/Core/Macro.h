#pragma once

#include "LogSystem.h"

//log macro
#define  LOG_TRACE(...) ::Tomato::LogSystem::LogTrace( __VA_ARGS__ )
#define  LOG_INFO(...) ::Tomato::LogSystem::LogInfo( __VA_ARGS__ )
#define  LOG_WARN(...) ::Tomato::LogSystem::LogWarn( __VA_ARGS__ )
#define  LOG_ERROR(...) ::Tomato::LogSystem::GetLoger()->error( __VA_ARGS__ )
#define  LOG_ASSERT(x, ...) if(!(x)){LOG_ERROR(__VA_ARGS__); __debugbreak();}
#define  ASSERT(x) if(!(x)){ __debugbreak();}


//event bind func
#define BIND_EVENT_FUNC(...) std::bind(&__VA_ARGS__, this, std::placeholders::_1)

#define NAME_OF(name) #name

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
	inline constexpr auto CreateRef(TArgs&&... args)
	{
		return std::make_shared<T>(std::forward<TArgs>(args)...);
	}

	template<typename T, typename Ptr>
	inline constexpr std::shared_ptr<T> As(const Ptr& ptr)
	{
		return std::dynamic_pointer_cast<T>(ptr);
	}

}
