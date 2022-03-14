#pragma once

#include "GRAVLibCore.h"
#include <stdexcept>

namespace GRAVLib::Concurrency::Threads::Exceptions
{
	class GRAVLibAPI threadException : public std::runtime_error
	{
	public:
		explicit threadException(const std::string& description);
		explicit threadException(const char* description);
		threadException(const threadException& other) noexcept;
		virtual ~threadException();
	};

}