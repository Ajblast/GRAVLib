#include "Concurrency/Fibers/Exceptions/FiberSwitchingException.h"

GRAVLib::Concurrency::Fibers::Exceptions::fiberSwitchingException::fiberSwitchingException(const std::string& description) : 
	fiberException(description) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberSwitchingException::fiberSwitchingException(const char* description) : 
	fiberException(description) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberSwitchingException::fiberSwitchingException(const fiberSwitchingException& other) noexcept :
	fiberException(other) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberSwitchingException::~fiberSwitchingException()
{}
