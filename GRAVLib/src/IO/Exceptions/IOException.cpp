#include "IO/Exceptions/IOException.h"

GRAVLib::IO::Exceptions::ioException::ioException(const std::string& description) noexcept : std::runtime_error(description)
{}
GRAVLib::IO::Exceptions::ioException::ioException(const char* description) noexcept : std::runtime_error(description) 
{}
GRAVLib::IO::Exceptions::ioException::ioException(const ioException& other) noexcept : std::runtime_error(other) 
{}
GRAVLib::IO::Exceptions::ioException::ioException(ioException&& other) noexcept : std::runtime_error("")
{
	*this = std::move(other);
}
GRAVLib::IO::Exceptions::ioException& GRAVLib::IO::Exceptions::ioException::operator=(const ioException& other) noexcept 
{
	std::runtime_error::operator= (other);

	if (this != &other)
	{

	}

	return *this;
}
GRAVLib::IO::Exceptions::ioException& GRAVLib::IO::Exceptions::ioException::operator=(ioException&& other) noexcept
{
	std::runtime_error::operator= (std::move(other));

	if (this != &other)
	{

	}

	return *this;

}
GRAVLib::IO::Exceptions::ioException::~ioException() 
{}
