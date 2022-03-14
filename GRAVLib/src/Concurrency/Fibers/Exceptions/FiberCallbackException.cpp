#include "Concurrency/Fibers/Exceptions/FiberCallbackException.h"

GRAVLib::Concurrency::Fibers::Exceptions::fiberCallbackException::fiberCallbackException(const std::string& description) :
	fiberException(description) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberCallbackException::fiberCallbackException(const char* description) : 
	fiberException(description)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberCallbackException::fiberCallbackException(const fiberCallbackException& other) noexcept : 
	fiberException(other)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberCallbackException::~fiberCallbackException()
{}
