#pragma once

#include "GRAVLibCore.h"
#include "FiberException.h"

namespace GRAVLib::Concurrency::Fibers::Exceptions
{
	class GRAVLibAPI fiberCallbackException : public fiberException
	{
	public:
		explicit fiberCallbackException(const std::string& description);
		explicit fiberCallbackException(const char* description);
		fiberCallbackException(const fiberCallbackException& other) noexcept;
		virtual ~fiberCallbackException();
	};
}