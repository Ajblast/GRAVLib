#include "Concurrency/Fibers/Exceptions/FiberCreationException.h"

GRAVLib::Concurrency::Fibers::Exceptions::fiberCreationException::fiberCreationException(const std::string& description) :
	fiberException(description) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberCreationException::fiberCreationException(const char* description) : 
	fiberException(description) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberCreationException::fiberCreationException(const fiberCreationException& other) noexcept :
	fiberException(other) 
{}
GRAVLib::Concurrency::Fibers::Exceptions::fiberCreationException::~fiberCreationException()
{}