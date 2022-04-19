#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Fibers/Exceptions/FiberException.h"

namespace GRAVLib::Concurrency::Fibers::Exceptions
{
		class GRAVLibAPI fiberCreationException : public fiberException
		{
		public:
			explicit fiberCreationException(const std::string& description);
			explicit fiberCreationException(const char* description);
			fiberCreationException(const fiberCreationException& other) noexcept;
			virtual ~fiberCreationException();
		};

	}
