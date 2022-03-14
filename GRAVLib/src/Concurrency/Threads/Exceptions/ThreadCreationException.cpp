#include "Concurrency/Threads/Exceptions/ThreadCreationException.h"

GRAVLib::Concurrency::Threads::Exceptions::threadCreationException::threadCreationException(const std::string& description) :
	threadException(description)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadCreationException::threadCreationException(const char* description) : 
	threadException(description)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadCreationException::threadCreationException(const threadCreationException& other) noexcept : 
	threadException(other)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadCreationException::~threadCreationException()
{}