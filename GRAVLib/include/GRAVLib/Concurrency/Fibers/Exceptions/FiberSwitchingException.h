#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Fibers/Exceptions/FiberException.h"

namespace GRAVLib::Concurrency::Fibers::Exceptions
{
	class GRAVLibAPI fiberSwitchingException : public fiberException
	{
	public:
		explicit fiberSwitchingException(const std::string& description);
		explicit fiberSwitchingException(const char* description);
		fiberSwitchingException(const fiberSwitchingException& other) noexcept;
		virtual ~fiberSwitchingException();
	};	
}