#include "IO/Files/TextFile.h"

GRAVLib::IO::textFile::textFile() : file()
{
}
GRAVLib::IO::textFile::textFile(const std::string& filePath, IO::fileMode fileMode, bool flushAfterWrite) : file(filePath, fileMode, flushAfterWrite)
{
}
GRAVLib::IO::textFile::textFile(textFile&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::IO::textFile& GRAVLib::IO::textFile::operator=(textFile&& other) noexcept
{
	file::operator=(std::move(other));

	return *this;
}
GRAVLib::IO::textFile::~textFile()
{
}

//bool GRAVLib::IO::textFile::readLine(char* buffer, size_t num, char delim)
//{
//	GRAV_ASSERT_TRUE(isOpen());
//	GRAV_ASSERT_TRUE(isInput());
//
//	if (endOfFile())
//		return false;
//
//	// Read a line from the stream
//	m_Stream.getline((char*)buffer, num, delim);
//
//	// Check if an error occured
//	errorHandle();
//
//	if (failed())
//		return false;
//
//	return true;
//}

bool GRAVLib::IO::textFile::readLine(std::string& str, char delim)
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to read line from unopened file
	GRAV_ASSERT_TRUE(isInput()); // Attempting to read line from non-input file

	if (endOfFile())
		return false;

	// Read a line
	std::getline(m_Stream, str, delim);

	// Check if an error occured
	errorHandle();

	if (failed())
		return false;

	return true;
}
bool GRAVLib::IO::textFile::readAll(std::string& str)
{
	GRAV_ASSERT_TRUE(isOpen());
	GRAV_ASSERT_TRUE(isInput());

	if (endOfFile())
		return false;

	scope<char[]> buffer;
	size_t bufferSize;

	bool readResult = file::readAll(buffer, bufferSize);
	if (readResult == false)
		return false;

	// Copy the generated buffer into another buffer +1 for /0
	scope<char[]> nBuffer = createScope<char[]>(bufferSize + 1);
	std::memcpy(nBuffer.get(), buffer.get(), bufferSize);
	nBuffer[bufferSize] = 0;

	// Create the string
	str = std::string(nBuffer.get());

	// Check if an error occured
	errorHandle();

	if (failed())
		return false;

	return true;
}

void GRAVLib::IO::textFile::write(std::string_view view)
{
	// Write the string and the null character
	file::write(view.data(), view.size());
}
void GRAVLib::IO::textFile::writeLine(std::string_view string)
{
	write(string);
	write("\n");
}

bool GRAVLib::IO::textFile::searchForToken(scope<const std::string[]> tokens, size_t tokenCount, size_t byteSearchDepth)
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to search for a token in an unopened file
	GRAV_ASSERT_TRUE(isInput()); // Attempting to search for a token in a non-input file

	// Move to the start of the file
	seekRead(0, seekOrigin::beg);

	// Create the buffer
	scope<char[]> buffer = createScope<char[]>(byteSearchDepth + 1);

	// Read from the file
	read(buffer, byteSearchDepth);

	// Precheck to see if anything was actually read
	if (readCount() == 0)
		return false;

	// Lowercase the bytes
	for (size_t i = 0; i < readCount(); i++)
		buffer[i] = (char)::tolower(buffer[i]);

	// Make the file be a C-string
	buffer[readCount()] = 0;

	std::string bufferString(buffer.get());
	std::string token;
	for (size_t i = 0; i < tokenCount; i++)
	{
		token = tokens[i];
		// Make sure the token is lowercase
		for (auto it = token.begin(); it != token.end(); it++)
			(*it) = (char)tolower(*it);

		// Search the buffer string for the token
		size_t pos = bufferString.find(token);
		if (pos == std::string::npos)
			continue;
		else
			return true;
	}

	return false;
}
