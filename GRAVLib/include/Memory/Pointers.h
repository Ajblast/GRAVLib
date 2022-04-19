#pragma once

#include "LibExportImport.h"
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


	//template<typename T>
	//class GRAVLibAPI scope
	//{
	//public:
	//	scope();
	//	scope(const scope& other) = delete;
	//	scope(scope&& other) noexcept;
	//	scope& operator=(const scope& other) = delete;
	//	scope& operator=(scope&& other) noexcept;

	//	T* release() noexcept
	//	{
	//		return m_Data.release();
	//	}
	//	void reset(T* pointer) noexcept
	//	{
	//		m_Data.reset(pointer);
	//	}
	//	void swap(scope& other) noexcept
	//	{
	//		m_Data.swap(other.m_Data);
	//	}

	//	T* get() noexcept
	//	{
	//		return m_Data.get();
	//	}
	//	explicit operator bool() const noexcept
	//	{
	//		return m_Data.operator bool();
	//	}
	//	
	//	std::add_lvalue_reference_t<T> operator*() const noexcept
	//	{
	//		return m_Data.operator*();
	//	}
	//	T* operator->() const noexcept
	//	{
	//		return m_Data.operator->();
	//	}
	//	T& operator[](size_t i) const
	//	{
	//		return m_Data.operator
	//	}
	//private:
	//	std::unique_ptr<T> m_Data;
	//};

	//template<typename T, typename... Args>
	//GRAVLibAPI constexpr scope<T> createScope(Args&& ...args)
	//{

	//}
}