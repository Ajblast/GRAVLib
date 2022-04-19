#include "GRAVLib/Concurrency/Fibers/Exceptions/FiberQuitCallbackException.h"

GRAVLib::Concurrency::Fibers::Exceptions::fiberQuitCallbackException::fiberQuitCallbackException(const std::string& description) : 
	fiberException(description)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberQuitCallbackException::fiberQuitCallbackException(const char* description) : 
	fiberException(description)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberQuitCallbackException::fiberQuitCallbackException(const fiberQuitCallbackException& other) noexcept :
	fiberException(other)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberQuitCallbackException::~fiberQuitCallbackException()
{}