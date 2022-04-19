#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/IO/Exceptions/IOException.h"
#include <string>

namespace GRAVLib::IO::Exceptions
{
	class GRAVLibAPI fileNotFoundException : public ioException
	{
	public:
		explicit fileNotFoundException(const char* fileName, const char* description) noexcept;
		explicit fileNotFoundException(const std::string& fileName, const std::string& description) noexcept;
		
		fileNotFoundException(const fileNotFoundException& other) noexcept;
		fileNotFoundException(fileNotFoundException&& other) noexcept;
		fileNotFoundException& operator= (const fileNotFoundException& other) noexcept;
		fileNotFoundException& operator= (fileNotFoundException&& other) noexcept;
		virtual ~fileNotFoundException() noexcept;

		const std::string& getFileName() const noexcept;


	private:
		std::string m_FileName;

	};
}