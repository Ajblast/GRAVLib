#pragma once

#include "GRAVLibCore.h"
#include <memory>

namespace GRAVLib
{
	template<typename T>
	using scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	GRAVLibAPI constexpr scope<T> createScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	GRAVLibAPI constexpr ref<T> createRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}