#include "GRAVLib/Concurrency/Fibers/Exceptions/FiberToThreadException.h"

GRAVLib::Concurrency::Fibers::Exceptions::fiberToThreadException::fiberToThreadException(const std::string& description) :
	fiberException(description)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberToThreadException::fiberToThreadException(const char* description) : 
	fiberException(description) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberToThreadException::fiberToThreadException(const fiberToThreadException& other) noexcept :
	fiberException(other)
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberToThreadException::~fiberToThreadException()
{}