#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Threads/Exceptions/ThreadException.h"

namespace GRAVLib::Concurrency::Threads::Exceptions
{
	class GRAVLibAPI threadAffinityException : public threadException
	{
	public:
		explicit threadAffinityException(const std::string& description);
		explicit threadAffinityException(const char* description);
		threadAffinityException(const threadAffinityException& other) noexcept;
		virtual ~threadAffinityException();
	};

}