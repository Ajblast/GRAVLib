#pragma once

#include "GRAVLibCore.h"
#include "FiberException.h"

namespace GRAVLib::Concurrency::Fibers::Exceptions
{
	class GRAVLibAPI fiberQuitCallbackException : public fiberException
	{
	public:
		explicit fiberQuitCallbackException(const std::string& description);
		explicit fiberQuitCallbackException(const char* description);
		fiberQuitCallbackException(const fiberQuitCallbackException& other) noexcept;
		virtual ~fiberQuitCallbackException();
	};
}