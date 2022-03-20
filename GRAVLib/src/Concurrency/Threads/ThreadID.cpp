#include "Concurrency/Threads/ThreadID.h"

GRAVLib::Concurrency::Threads::threadID::threadID() :
	m_ThreadID(GRAVLib_INVALID_THREAD_ID), m_Handle(nullptr)
{}
GRAVLib::Concurrency::Threads::threadID::threadID(threadID_t threadID, threadHandle_t handle) :
	m_ThreadID(threadID), m_Handle(handle)
{}