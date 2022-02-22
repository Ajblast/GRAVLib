#pragma once

#include <string>
#include "GRAVLibCore.h"
#include "File.h"

namespace GRAVLib::IO
{
	class GRAVLibAPI textFile : public file
	{
	public:
		textFile();
		textFile(const std::string& filePath, IO::fileMode fileMode, bool flushAfterWrite);
		textFile(const textFile& other) = delete;
		textFile(textFile&& other) noexcept;
		textFile& operator= (const textFile& other) = delete;
		textFile& operator= (textFile&& other) noexcept;
		virtual ~textFile();

		// Read a string into buffer spliting by delim. A null character is automatically appended.
		bool readLine(std::string& str, char delim = '\n');
		// Read the entire file into the string
		bool readAll(std::string& str);

		// Write a string to the file
		void write(const std::string& string);
		// Write a string and append a newline character to the end of the string
		void writeLine(const std::string& string);

		// Search the header of the file for a specific token.
		bool searchForToken(scope<const std::string[]> tokens, size_t tokencount, size_t byteSearchDepth = 200);
	};

}