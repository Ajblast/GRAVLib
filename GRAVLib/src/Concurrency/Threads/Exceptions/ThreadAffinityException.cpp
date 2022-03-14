#include "Concurrency/Threads/Exceptions/ThreadAffinityException.h"

GRAVLib::Concurrency::Threads::Exceptions::threadAffinityException::threadAffinityException(const std::string& description) : 
	threadException(description)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadAffinityException::threadAffinityException(const char* description) :
	threadException(description)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadAffinityException::threadAffinityException(const threadAffinityException& other) noexcept : 
	threadException(other)
{}
GRAVLib::Concurrency::Threads::Exceptions::threadAffinityException::~threadAffinityException()
{}