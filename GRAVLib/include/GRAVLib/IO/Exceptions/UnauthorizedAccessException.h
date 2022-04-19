#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/IO/Exceptions/IOException.h"
#include <string>

namespace GRAVLib::IO::Exceptions
{
	class GRAVLibAPI unauthorizedAccessException : public ioException
	{
	public:
		explicit unauthorizedAccessException(const char* fileName, const char* description);
		explicit unauthorizedAccessException(const std::string& fileName, const std::string& description);
		
		unauthorizedAccessException(const unauthorizedAccessException& other) noexcept;
		unauthorizedAccessException(unauthorizedAccessException&& other) noexcept;
		unauthorizedAccessException& operator= (const unauthorizedAccessException& other) noexcept;
		unauthorizedAccessException& operator= (unauthorizedAccessException&& other) noexcept;
		virtual ~unauthorizedAccessException() noexcept;

		inline const std::string& getFileName() const noexcept { return m_FileName; }

	private:
		std::string m_FileName;
	};

}