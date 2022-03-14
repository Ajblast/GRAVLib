#include "Concurrency/Threads/ThreadID.h"

GRAVLib::Concurrency::Threads::threadID::threadID() :
	m_ThreadID(GRAVLib_INVALID_THREAD_ID), m_Handle(nullptr), m_Index(GRAVLib_INVALID_THREAD_INDEX)
{}
GRAVLib::Concurrency::Threads::threadID::threadID(threadID_t threadID, threadHandle_t handle, threadIndex_t index) :
	m_ThreadID(threadID), m_Handle(handle), m_Index(index)
{}
GRAVLib::Concurrency::Threads::threadID::threadID(const threadID& other) :
	m_ThreadID(other.m_ThreadID), m_Handle(other.m_Handle), m_Index(other.m_Index)
{}
GRAVLib::Concurrency::Threads::threadID::threadID(threadID&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Concurrency::Threads::threadID& GRAVLib::Concurrency::Threads::threadID::operator=(const threadID& other)
{
	if (this != &other)
	{
		m_ThreadID = other.m_ThreadID;
		m_Handle = other.m_Handle;
		m_Index = other.m_Index;
	}

	return *this;
}
GRAVLib::Concurrency::Threads::threadID& GRAVLib::Concurrency::Threads::threadID::operator=(threadID&& other) noexcept
{
	if (this != &other)
	{
		m_ThreadID = std::move(other.m_ThreadID);
		m_Handle = std::move(other.m_Handle);
		m_Index = std::move(other.m_Index);
	}

	return *this;
}