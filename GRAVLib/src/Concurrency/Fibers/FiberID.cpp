#include "GRAVLib/Concurrency/Fibers/FiberID.h"

GRAVLib::Concurrency::Fibers::fiberID::fiberID() :
	m_Handle(nullptr)
{}
GRAVLib::Concurrency::Fibers::fiberID::fiberID(fiberHandle_t handle) :
	m_Handle(handle)
{}