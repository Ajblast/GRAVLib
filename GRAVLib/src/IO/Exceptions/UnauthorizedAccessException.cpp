#include "IO/Exceptions/UnauthorizedAccessException.h"

GRAVLib::IO::Exceptions::unauthorizedAccessException::unauthorizedAccessException(const char* fileName, const char* description) : 
	unauthorizedAccessException(std::string(fileName), std::string(description))
{}
GRAVLib::IO::Exceptions::unauthorizedAccessException::unauthorizedAccessException(const std::string& fileName, const std::string& description) : 
	m_FileName(fileName), ioException(description)
{}
GRAVLib::IO::Exceptions::unauthorizedAccessException::unauthorizedAccessException(const unauthorizedAccessException& other) noexcept : 
	m_FileName(other.m_FileName), ioException(other)
{}
GRAVLib::IO::Exceptions::unauthorizedAccessException::unauthorizedAccessException(unauthorizedAccessException&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::IO::Exceptions::unauthorizedAccessException::unauthorizedAccessException& GRAVLib::IO::Exceptions::unauthorizedAccessException::operator= (const unauthorizedAccessException& other) noexcept
{
	ioException::operator=(other);

	if (this != &other)
	{
		m_Filename = other.m_FileName;
	}

	return *this;
}
GRAVLib::IO::Exceptions::unauthorizedAccessException::unauthorizedAccessException& GRAVLib::IO::Exceptions::unauthorizedAccessException::operator= (unauthorizedAccessException&& other) noexcept
{
	ioException::operator=(std::move(other));

	if (this != &other)
	{
		m_FileName = std::move(other.m_FileName);
	}

	return *this;
}
GRAVLib::IO::Exceptions::unauthorizedAccessException::~unauthorizedAccessException() noexcept
{
}