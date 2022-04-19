#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include <string>
#include <stdexcept>

namespace GRAVLib::Concurrency::Fibers::Exceptions
{
	class GRAVLibAPI fiberException : public std::runtime_error
	{
	public:
		explicit fiberException(const std::string& description);
		explicit fiberException(const char* description);
		fiberException(const fiberException& other) noexcept;
		virtual ~fiberException();
	};

}