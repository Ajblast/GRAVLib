// TODO: File Tests
#include <gtest/gtest.h>
#include "GRAVLib/IO/Files/File.h"
#include "GRAVLib/IO/Exceptions/IOException.h"

namespace GRAVLib::IO
{
	// Check if opening with an invalid file path
	TEST(Files, WillNotOpenWithInvalidFilePath)
	{
		try
		{
			file sut = file("@", fileMode::INPUT, false);

			FAIL() << "Exception not thrown while opening invalid file path";
		}
		catch (Exceptions::ioException)
		{
			SUCCEED() << "Exception thrown while opening invalid file path.";
		}
	}
	// Check if null file can be initialized
	TEST(Files, CanBeDefaultConstructed)
	{
		file sut = file();

		ASSERT_FALSE(sut.isOpen()) << "File is not opened.";
		ASSERT_FALSE(sut.endOfFile()) << "File opened at the end of the file";
		ASSERT_FALSE(sut.failed()) << "File failed its last operation.";

		ASSERT_EQ(sut.fileMode(), fileMode::NONE) << "File has a non null file mode.";
		ASSERT_EQ(sut.filePath(), "") << "Default constructed file doesn't have an empty string file path.";
	}
	
	class File : public ::testing::Test {
	public:
		File()
		{
		}
		virtual ~File() override
		{
		}
	protected:
		void SetUp() override
		{
			sut = file(m_FilePath, m_FileMode, false);
		}
		void TearDown() override
		{
		}

		file sut;
		std::string m_FilePath = std::string(GRAVLibTests_PATH) + "/IO/Files/testFile.txt";
		fileMode m_FileMode = fileMode::INPUT | fileMode::OUTPUT | fileMode::BINARY;
	};

	// Check can reopen file in different mode
	TEST_F(File, CanReopenInDifferentFileMode)
	{
		ASSERT_TRUE(sut.isOpen()) << "Unable to open file.";
		ASSERT_EQ(sut.filePath(), m_FilePath) << "File paths are not equal.";
		EXPECT_EQ(sut.fileMode(), m_FileMode) << "File mode is not correct starting filemode.";
		EXPECT_FALSE(sut.endOfFile()) << "File started at eof of nonempty file.";
		EXPECT_FALSE(sut.failed()) << "File failed after opening";

		fileMode newMode = fileMode::INPUT;
		sut.reopen(newMode, false);

		ASSERT_TRUE(sut.isOpen()) << "Unable to open file.";
		ASSERT_EQ(sut.filePath(), m_FilePath) << "File paths are not equal.";
		EXPECT_EQ(sut.fileMode(), newMode) << "File mode is not correct starting filemode.";
		EXPECT_FALSE(sut.endOfFile()) << "File started at eof of nonempty file.";
		EXPECT_FALSE(sut.failed()) << "File failed after opening";
	}

	// Check can reopen after closing
	TEST_F(File, CanReopenAfterClosing)
	{
		ASSERT_TRUE(sut.isOpen()) << "Unable to open file.";
		ASSERT_EQ(sut.filePath(), m_FilePath) << "File paths are not equal.";
		EXPECT_EQ(sut.fileMode(), m_FileMode) << "File mode is not correct starting filemode.";
		EXPECT_FALSE(sut.endOfFile()) << "File started at eof of nonempty file.";
		EXPECT_FALSE(sut.failed()) << "File failed after opening";

		// Close the file
		sut.close();
		ASSERT_FALSE(sut.isOpen()) << "File was still open after closing.";
		ASSERT_EQ(sut.filePath(), m_FilePath) << "File paths are not equal.";
		EXPECT_EQ(sut.fileMode(), fileMode::NONE) << "File mode was not removed after closing.";
		EXPECT_FALSE(sut.failed()) << "File failed after closing";

		// Reopen the file again with a new filemode
		fileMode newMode = fileMode::INPUT;
		sut.reopen(newMode, false);

		ASSERT_TRUE(sut.isOpen()) << "Unable to open file.";
		ASSERT_EQ(sut.filePath(), m_FilePath) << "File paths are not equal.";
		EXPECT_EQ(sut.fileMode(), newMode) << "File mode is not correct starting filemode.";
		EXPECT_FALSE(sut.endOfFile()) << "File started at eof of nonempty file.";
		EXPECT_FALSE(sut.failed()) << "File failed after opening";
	}

	// Check seeking for all origins
	TEST_F(File, CanSeekForAllOrigins)
	{
		ASSERT_TRUE(sut.isOpen()) << "Unable to open file.";
		ASSERT_EQ(sut.filePath(), m_FilePath) << "File paths are not equal.";
		EXPECT_EQ(sut.fileMode(), m_FileMode) << "File mode is not correct starting filemode.";
		EXPECT_FALSE(sut.endOfFile()) << "File started at eof of nonempty file.";
		EXPECT_FALSE(sut.failed()) << "File failed after opening";


		// Seek beginning
		EXPECT_TRUE(sut.seekRead(0, seekOrigin::beg)) << "Seeking to beginning of file failed.";
		EXPECT_EQ(sut.offsetRead(), 0) << "Read offset was not 0.";
		EXPECT_FALSE(sut.endOfFile()) << "Read offset was at the end of the file.";

		// Seek ending
		EXPECT_TRUE(sut.seekRead(0, seekOrigin::end)) << "Seeking to end of file failed.";
		EXPECT_EQ(sut.offsetRead(), sut.fileSize()) << "Read offset was not the end of the file.";
		EXPECT_FALSE(sut.endOfFile()) << "Read offset was not the end of the file.";

		// Seek current
		size_t currentSeekRead = sut.offsetRead();
		EXPECT_TRUE(sut.seekRead(0, seekOrigin::cur)) << "Seeking to current position of file failed.";
		EXPECT_EQ(sut.offsetRead(), currentSeekRead) << "Read offset was not the current read seek position.";
		EXPECT_FALSE(sut.endOfFile()) << "Read offset was not the end of the file when seeking to current position at the endo fthe file.";


		// Seek beginning
		EXPECT_TRUE(sut.seekWrite(0, seekOrigin::beg)) << "Seeking to beginning of file failed.";
		EXPECT_EQ(sut.offsetWrite(), 0) << "Write offset was not 0.";
		EXPECT_FALSE(sut.endOfFile()) << "Write offset was at the end of the file.";

		// Seek ending
		EXPECT_TRUE(sut.seekWrite(0, seekOrigin::end)) << "Seeking to end of file failed.";
		EXPECT_EQ(sut.offsetWrite(), sut.fileSize()) << "Write offset was not the end of the file.";
		EXPECT_FALSE(sut.endOfFile()) << "Write offset was not the end of the file.";

		// Seek current
		size_t currentSeekWrite = sut.offsetWrite();
		EXPECT_TRUE(sut.seekWrite(0, seekOrigin::cur)) << "Seeking to current position of file failed.";
		EXPECT_EQ(sut.offsetWrite(), currentSeekWrite) << "Write offset was not the current write seek position.";
		EXPECT_FALSE(sut.endOfFile()) << "Write offset was not the end of the file when seeking to current position at the endo fthe file.";
	}
	
	// Check filesize
	TEST_F(File, CanReadProperFileSize)
	{
		ASSERT_TRUE(sut.isOpen()) << "Unable to open file.";
		ASSERT_EQ(sut.filePath(), m_FilePath) << "File paths are not equal.";

		EXPECT_EQ(sut.fileMode(), m_FileMode) << "File mode is not input";
		EXPECT_EQ(sut.fileSize(), 68) << "TestFile size is not equal.";
	}

	// Check can read
	TEST_F(File, CanBeReadFrom)
	{	
		ASSERT_TRUE(sut.isOpen()) << "Unable to open file.";
		ASSERT_EQ(sut.fileMode(), m_FileMode) << "File mode is not input";
		ASSERT_EQ(sut.filePath(), m_FilePath) << "File path is not the same as the given path.";

		EXPECT_FALSE(sut.endOfFile()) << "File opened at the end of the file.";
		EXPECT_FALSE(sut.failed()) << "File IO operation failed.";

		EXPECT_EQ(sut.offsetRead(), 0) << "File read offset did not start at the beginning of the file when constructed.";


		// Check read into buffer
		size_t bufferSize = 22;
		scope<char[]> buffer = createScope<char[]>(bufferSize);
		buffer[bufferSize - 1] = 0; // Make it a c-string

		sut.read(buffer, bufferSize - 1);	// Save space for null character

		EXPECT_EQ(sut.readCount(), bufferSize - 1) << "Did not read " << bufferSize - 1 << " characters from the file.";
		EXPECT_EQ(sut.offsetRead(), bufferSize - 1) << "Read offset did not change after reading from file.";
		EXPECT_FALSE(memcmp(buffer.get(), "Test file first line.", bufferSize)) << "Read in buffer did not match the first " << bufferSize << " characters of the file.";


		// Check peek character
		EXPECT_TRUE(sut.seekRead(0, seekOrigin::beg)) << "Unable to seek to the beginning of the file.";
		EXPECT_EQ(sut.offsetRead(), 0) << "File read offset was not set to 0.";
		EXPECT_EQ(sut.peek(), 'T') << "Peek of the first character of the test file did not match what was read.";


		// Check read character
		EXPECT_TRUE(sut.seekRead(0, seekOrigin::beg)) << "Unable to seek to the beginning of the file.";
		EXPECT_EQ(sut.offsetRead(), 0) << "File read offset was not set to 0.";
		EXPECT_EQ(sut.readChar(), 'T') << "Peek of the first character of the test file did not match what was read.";
		EXPECT_EQ(sut.offsetRead(), 1) << "File read offset was not set to 1 after read character.";


		// Check reading entire file
		EXPECT_TRUE(sut.readAll(buffer, bufferSize)) << "Unable to read in entire file. Only " << sut.readCount() << " were read.";
		EXPECT_FALSE(memcmp(buffer.get(), "Test file first line.\r\nTest file second line.\r\nTest File third line.", bufferSize)) << "Read in file did not match contents.";
	}

	// Check can write
	TEST_F(File, CanBeWrittenTo)
	{
		FAIL();

		// Check can write to file
		// Check can write character to file
		// Check flush
	}



}