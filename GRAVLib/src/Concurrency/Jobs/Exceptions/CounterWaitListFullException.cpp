#include "Concurrency/Jobs/Exceptions/CounterWaitListFullException.h"

GRAVLib::Concurrency::Jobs::Exceptions::counterWaitListFullException::counterWaitListFullException(const std::string& description) : 
	std::runtime_error(description)
{}
GRAVLib::Concurrency::Jobs::Exceptions::counterWaitListFullException::counterWaitListFullException(const char* description) :
	std::runtime_error(description) 
{}
GRAVLib::Concurrency::Jobs::Exceptions::counterWaitListFullException::counterWaitListFullException(const counterWaitListFullException& other) noexcept :
	std::runtime_error(other)
{}
GRAVLib::Concurrency::Jobs::Exceptions::counterWaitListFullException::~counterWaitListFullException()
{}
