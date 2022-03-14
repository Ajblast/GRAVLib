#include "Concurrency/Fibers/FiberID.h"

GRAVLib::Concurrency::Fibers::fiberID::fiberID() :
	m_Handle(nullptr), m_Index(GRAVLib_INVALID_FIBER_INDEX)
{}
GRAVLib::Concurrency::Fibers::fiberID::fiberID(fiberHandle_t handle, fiberIndex_t index) :
	m_Handle(handle), m_Index(index)
{}
GRAVLib::Concurrency::Fibers::fiberID::fiberID(const fiberID& other) :
	m_Handle(other.m_Handle), m_Index(other.m_Index)
{}
GRAVLib::Concurrency::Fibers::fiberID::fiberID(fiberID&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Concurrency::Fibers::fiberID& GRAVLib::Concurrency::Fibers::fiberID::operator=(const fiberID& other)
{
	if (this != &other)
	{
		m_Handle = other.m_Handle;
		m_Index = other.m_Index;
	}

	return *this;
}
GRAVLib::Concurrency::Fibers::fiberID& GRAVLib::Concurrency::Fibers::fiberID::operator=(fiberID&& other) noexcept
{
	if (this != &other)
	{
		m_Handle = std::move(other.m_Handle);
		m_Index = std::move(other.m_Index);
	}

	return *this;
}