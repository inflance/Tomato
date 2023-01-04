#pragma once

#include "LogSystem.h"

//log macro
#define  LOG_TRACE(...) ::Tomato::LogSystem::GetLoger()->trace(__VA_ARGS__)
#define  LOG_INFO(...) ::Tomato::LogSystem::GetLoger()->info(__VA_ARGS__)
#define  LOG_WARN(...) ::Tomato::LogSystem::GetLoger()->warn(__VA_ARGS__)
#define  LOG_ERROR(...) ::Tomato::LogSystem::GetLoger()->error(__VA_ARGS__)
#define  LOG_ASSERT(x, ...) if(!(x)){LOG_ERROR(__VA_ARGS__); __debugbreak();}

//event bind func
#define BIND_EVENT_FUNC(...) std::bind(&__VA_ARGS__, this, std::placeholders::_1)


#include <memory>

namespace Tomato {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}