#include "Concurrency/Threads/Exceptions/ThreadException.h"

GRAVLib::Concurrency::Threads::Exceptions::threadException::threadException(const std::string& description) : 
	std::runtime_error(description)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadException::threadException(const char* description) : 
	std::runtime_error(description)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadException::threadException(const threadException& other) noexcept :
	std::runtime_error(other)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadException::~threadException()
{}
