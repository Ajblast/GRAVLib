#pragma once

#include "GRAVLibCore.h"
#include <stdexcept>

namespace GRAVLib::Concurrency::Jobs::Exceptions
{
	class GRAVLibAPI counterWaitListFullException : public std::runtime_error
	{
	public:
		explicit counterWaitListFullException(const std::string& description);
		explicit counterWaitListFullException(const char* description);
		counterWaitListFullException(const counterWaitListFullException& other) noexcept;
		virtual ~counterWaitListFullException();
	};
}