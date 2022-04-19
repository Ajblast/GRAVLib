#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include <string>
#include <stdexcept>

namespace GRAVLib::IO::Exceptions
{
	class GRAVLibAPI ioException : public std::runtime_error
	{
	public:
		explicit ioException(const std::string& description) noexcept;
		explicit ioException(const char* description) noexcept;
		
		ioException(const ioException& other) noexcept;
		ioException(ioException&& other) noexcept;
		ioException& operator=(const ioException& other) noexcept;
		ioException& operator=(ioException&& other) noexcept;
		virtual ~ioException();
	};


	//class directoryNotFoundException : public ioException
	//{
	//
	//};
	//
	//class driveNotFoundException : public ioException
	//{
	//
	//};
	//
	//class pathTooLongException : public ioException
	//{
	//
	//};
	//
	//class unauthorizedAccessException : public ioException
	//{
	//
	//};
}