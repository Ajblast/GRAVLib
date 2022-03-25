#pragma once

#include "GRAVLibCore.h"
#include <stdexcept>

namespace GRAVLib::Concurrency::Jobs::Exceptions
{
	class GRAVLibAPI readyFiberException : public std::runtime_error
	{
	public:
		explicit readyFiberException(const std::string& description);
		explicit readyFiberException(const char* description);
		readyFiberException(const readyFiberException& other) noexcept;
		virtual ~readyFiberException();
	};
}