#include "Concurrency/Fibers/Exceptions/FiberException.h"

GRAVLib::Concurrency::Fibers::Exceptions::fiberException::fiberException(const std::string& description) : 
	std::runtime_error(description)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberException::fiberException(const char* description) : 
	std::runtime_error(description)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberException::fiberException(const fiberException& other) noexcept :
	std::runtime_error(other) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberException::~fiberException()
{}