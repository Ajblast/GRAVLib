#include "IO/Exceptions/FileNotFoundException.h"

GRAVLib::Exceptions::IO::fileNotFoundException::fileNotFoundException(const char* fileName, const char* description) noexcept :
	fileNotFoundException(std::string(fileName), std::string(description))
{}
GRAVLib::Exceptions::IO::fileNotFoundException::fileNotFoundException(const std::string& fileName, const std::string& description) noexcept : 
	m_FileName(fileName), ioException(description)
{}

GRAVLib::Exceptions::IO::fileNotFoundException::fileNotFoundException(const fileNotFoundException& other) noexcept :
	m_FileName(other.m_FileName), ioException(other)
{}
GRAVLib::Exceptions::IO::fileNotFoundException::fileNotFoundException(fileNotFoundException&& other) noexcept
{
	*this = std::move(other);
}

GRAVLib::Exceptions::IO::fileNotFoundException& GRAVLib::Exceptions::IO::fileNotFoundException::operator=(const fileNotFoundException& other) noexcept
{
	ioException::operator=(other);

	if (this != &other)
	{
		m_FileName = other.m_FileName;
	}

	return *this;
}
GRAVLib::Exceptions::IO::fileNotFoundException& GRAVLib::Exceptions::IO::fileNotFoundException::operator=(fileNotFoundException&& other) noexcept
{
	ioException::operator=(std::move(other));

	if (this != &other)
	{
		m_FileName = std::move(other.m_FileName);
	}

	return *this;
}
GRAVLib::Exceptions::IO::fileNotFoundException::~fileNotFoundException()
{
	
}

const std::string& GRAVLib::Exceptions::IO::fileNotFoundException::getFileName() const noexcept
{
	return m_FileName;
}
