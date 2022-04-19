#include "GRAVLib/IO/Files/File.h"
#include "GRAVLib/IO/Exceptions/IOException.h"

#include <stdexcept>

GRAVLib::IO::file::file() : m_FilePath(), m_FileMode(fileMode::NONE), m_FlushAfterWrite(false), m_Stream()
{}
GRAVLib::IO::file::file(const std::string& filePath, GRAVLib::IO::fileMode fileMode, bool flushAfterWrite)
{
	// Open the file
	open(filePath, fileMode, flushAfterWrite);
}
GRAVLib::IO::file::file(file&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::IO::file& GRAVLib::IO::file::operator=(file&& other) noexcept
{
	if (this != &other)
	{
		// Close the current file
		close();

		// Steal resources
		m_FilePath = std::move(other.m_FilePath);
		m_Stream = std::move(other.m_Stream);
		m_FileMode = std::move(other.m_FileMode);
		m_FlushAfterWrite = std::move(other.m_FlushAfterWrite);
	}

	return *this;
}

GRAVLib::IO::file::~file()
{
	// Close the file handle
	close();
}

void GRAVLib::IO::file::open(const std::string& filePath, IO::fileMode fileMode, bool flushAfterWrite)
{
	GRAV_ASSERT_FALSE(isOpen());

	// Set file mode and file size
	m_FilePath = filePath;
	m_FileMode = fileMode;
	m_FlushAfterWrite = flushAfterWrite;

	// Check if the file directory exists, and create it if needed if it is a write operation
	if ((fileMode & fileMode::OUTPUT) == fileMode::OUTPUT && IO::exists(filePath) == false)
		IO::createDirectory(std::filesystem::path(filePath).remove_filename());

	// Opening the file
	m_Stream.open(filePath, (std::fstream::openmode)fileMode);

	// Check for an error
	errorHandle();

	// Check if opening the file failed
	if (failed())
		throw IO::Exceptions::ioException("Unable to open file.");

	// The file is now open here
}
void GRAVLib::IO::file::reopen(IO::fileMode fileMode, bool flushAfterWrite)
{
	// Close the current file first if it is open
	close();

	// Reopen the file with new settings
	open(m_FilePath, fileMode, flushAfterWrite);
}
void GRAVLib::IO::file::close()
{
	if (isOpen())
	{
		// Reset the state bits because close doesn't clear the bits and previous failures can pass over
		m_Stream.clear();

		// Close the stream
		m_Stream.close();

		// Check for any errors
		errorHandle();

		// The file was not able to be closed.
		if (failed())
			throw Exceptions::ioException("Failure to close file: " + m_FilePath);

		// Null out everything
		m_FileMode = fileMode::NONE;
	}
}

bool GRAVLib::IO::file::seekWrite(long offset, seekOrigin origin)
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to seek write in an unopened file
	
	if (isOpen() == false)
		return false;

	// Seek the write position
	m_Stream.seekp(offset, (std::ios_base::seekdir)origin);

	// Check if there is an error with the stream
	errorHandle();

	// Return false if it could not seek to the position
	if (failed())
		return false;

	// Default return true
	return true;
}
bool GRAVLib::IO::file::seekRead(long offset, seekOrigin origin)
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to seek read in an unopened file

	if (isOpen() == false)
		return false;

	// Seek the input position
	m_Stream.seekg(offset, (std::ios_base::seekdir)origin);

	// Check if there is an error with the stream
	errorHandle();

	// Return false if it could not seek to the position
	if (failed())
		return false;

	// Default return true
	return true;
}

bool GRAVLib::IO::file::flush()
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to flush in an unopened file

	if (isOpen() == false)
		return false;

	// flush the stream
	m_Stream.flush();

	// Check if an error occured
	errorHandle();

	// Check if the flush failed
	if (failed())
		return false;

	// Default return tue
	return true;
}

const size_t GRAVLib::IO::file::offsetWrite()
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to get write offset in an unopened file
	GRAV_ASSERT_TRUE(isOutput()); // Attempting to get write offset in a non-output file

	size_t position = m_Stream.tellp();

	errorHandle();

	return position;
}
const size_t GRAVLib::IO::file::offsetRead()
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to get read offset in an unopened file
	GRAV_ASSERT_TRUE(isInput()); // Attempting to get read offset in a non-input file

	size_t position = m_Stream.tellg();

	errorHandle();

	return position;
}
const size_t GRAVLib::IO::file::fileSize()
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to get file size in an unopened file
	GRAV_ASSERT_TRUE(isInput() || isOutput()); // Attempting to get file size in an non-input or non-output file

	size_t fileSize;

	if (isInput())
	{
		// Cache the current offset
		size_t currentPosition = offsetRead();

		// Go the end of the file
		if (seekRead(0, seekOrigin::end) == false)
			throw Exceptions::ioException("Unable to seek read to beginning of file.");
		fileSize = offsetRead();

		// Return back to the cached position
		if (seekRead(currentPosition, seekOrigin::beg) == false)
			throw Exceptions::ioException("Unable to seek to last read position.");
	}
	else
	{
		// Cache the current offset
		size_t currentPosition = offsetWrite();

		// Go the end of the file
		if (seekWrite(0, seekOrigin::end) == false)
			throw Exceptions::ioException("Unable to seek write to beginning of file.");
		fileSize = offsetWrite();

		// Return back to the cached position
		if (seekWrite(currentPosition, seekOrigin::beg) == false)
			throw Exceptions::ioException("Unable to seek to last write position.");
	}

	return fileSize;
}

size_t GRAVLib::IO::file::readCount()
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to get the read count in an unopened file
	GRAV_ASSERT_TRUE(isInput()); // Attempting to get the read count in a non-input file

	// Get the number of characters read
	return m_Stream.gcount();
}
int GRAVLib::IO::file::peek()
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to peek in an unopened file
	GRAV_ASSERT_TRUE(isInput()); // Attempting to peek in a non-input file

	// Peek the character
	int character = m_Stream.peek();

	// Check for an error
	errorHandle();

	return character;
}
bool GRAVLib::IO::file::read(scope<char[]>& buffer, size_t bufferSize)
{
	GRAV_ASSERT_NE(buffer, nullptr); // Attempting to read into a null buffer
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to read from an unopend file
	GRAV_ASSERT_TRUE(isInput()); // Attempting to read on a non-input file

	// Do nothing if nowhere to put data
	if (bufferSize == 0)
	{
		// Reset the readcount to 0
		peek();
		// This is not a failure
		return true;
	}

	// Read into the buffer bufferSize of characters
	m_Stream.read(buffer.get(), bufferSize);

	// Check if an error occured
	errorHandle();

	if (failed())
		return false;

	return true;
}
int GRAVLib::IO::file::readChar()
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to read char from an unopened file
	GRAV_ASSERT_TRUE(isInput()); // Attempting to read char fram a non-input file

	// Get a character
	int character = m_Stream.get();

	// Get error code from reading
	errorHandle();

	return character;
}
bool GRAVLib::IO::file::readAll(scope<char[]>& buffer, size_t& bufferSize)
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to read from an unopened file
	GRAV_ASSERT_TRUE(isInput()); // Attempting to read from a non-input file

	// Set the buffer size
	bufferSize = fileSize();

	// Allocate the new buffer
	buffer = createScope<char[]>(bufferSize);

	// Seek to the beginning of the file
	if (seekRead(0, seekOrigin::beg) == false)
		throw Exceptions::ioException("Unable to seek to the beginning of file.");

	// Read into the buffer
	bool readResult = read(buffer, bufferSize);
	bool eof = endOfFile();

	// The entire file was read if read fails and eof is set, or the read result was already true
	if (readResult)
		return true;
	else if (readResult == false && eof)
		return true;
	else 
		return false;
}


void GRAVLib::IO::file::write(const char* buffer, size_t bufferSize)
{
	GRAV_ASSERT_NE(buffer, nullptr); // Buffer is not initialized
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to write to an unopened file
	GRAV_ASSERT_TRUE(isOutput()); // Attempting to write to a non-output file

	// Write to the stream
	m_Stream.write(buffer, bufferSize);

	// Check if an error occured
	errorHandle();

	// Flush if it should flush after every write
	if (m_FlushAfterWrite)
		flush();
}
void GRAVLib::IO::file::write(scope<const char[]>& buffer, size_t bufferSize)
{
	GRAV_ASSERT_NE(buffer, nullptr); // Buffer is not initialized
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to write to an unopened file
	GRAV_ASSERT_TRUE(isOutput()); // Attempting to write to a non-output file

	write(buffer.get(), bufferSize);
}
void GRAVLib::IO::file::writeChar(char character)
{
	GRAV_ASSERT_TRUE(isOpen()); // Attempting to write char to unopened file
	GRAV_ASSERT_TRUE(isOutput()); // Attempting to write char to a non-output file

	// Write to the stream
	m_Stream.put(character);

	// Check if an error occured
	errorHandle();

	// Flush if it should flush after every write
	if (m_FlushAfterWrite)
		flush();
}

void GRAVLib::IO::file::errorHandle()
{
	// Check if the stream is bad for any reason
	if (m_Stream.bad())
		throw IO::Exceptions::ioException("File operation error");
}
