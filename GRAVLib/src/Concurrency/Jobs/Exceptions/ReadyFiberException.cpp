#include "Concurrency/Jobs/Exceptions/readyFiberException.h"

GRAVLib::Concurrency::Jobs::Exceptions::readyFiberException::readyFiberException(const std::string& description) : 
	std::runtime_error(description)
{}
GRAVLib::Concurrency::Jobs::Exceptions::readyFiberException::readyFiberException(const char* description) :
	std::runtime_error(description) 
{}
GRAVLib::Concurrency::Jobs::Exceptions::readyFiberException::readyFiberException(const readyFiberException& other) noexcept :
	std::runtime_error(other)
{}
GRAVLib::Concurrency::Jobs::Exceptions::readyFiberException::~readyFiberException()
{}
