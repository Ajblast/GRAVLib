#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Threads/Exceptions/ThreadException.h"

namespace GRAVLib::Concurrency::Threads::Exceptions
{
	class GRAVLibAPI threadCreationException : public threadException
	{
	public:
		explicit threadCreationException(const std::string& description);
		explicit threadCreationException(const char* description);
		threadCreationException(const threadCreationException& other) noexcept;
		virtual ~threadCreationException();
	};

}