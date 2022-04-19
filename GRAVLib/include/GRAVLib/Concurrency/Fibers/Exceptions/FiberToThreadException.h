#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Fibers/Exceptions/FiberException.h"

namespace GRAVLib::Concurrency::Fibers::Exceptions
{
	class GRAVLibAPI fiberToThreadException : public fiberException
	{
	public:
		explicit fiberToThreadException(const std::string& description);
		explicit fiberToThreadException(const char* description);
		fiberToThreadException(const fiberToThreadException& other) noexcept;
		virtual ~fiberToThreadException();
	};
}