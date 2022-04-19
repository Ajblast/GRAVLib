#include <gtest/gtest.h>
#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Threads/Thread.h"

namespace GRAVLib::Concurrency::Threads
{
	static bool threadFunctionValue = false;
	unsigned int threadFunction(thread* thread)
	{
		threadFunctionValue = true;
		return 0;
	}

	TEST(Threads, CanBeInitializedFromCurrentThread)
	{
		thread sut;
		sut.initializeFromCurrentThread();
		
		// Check that the thread was valid
		ASSERT_TRUE(sut.valid()) << "Initialzed thread was not valid.";

		#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
		EXPECT_NE(sut.getThreadHandle(), nullptr) << "Windows thread native handle was not initialized.";
		#endif

		sut.close();
	}
	TEST(Threads, CanBeCreatedWithFreeFunction)
	{
		ASSERT_FALSE(threadFunctionValue) << "Thread function value was not initially false.";

		thread::threadCallback callbackFunction = threadFunction;
		thread sut(callbackFunction);


		// Check that the thread was valid
		ASSERT_TRUE(sut.valid()) << "Initialzed thread was not valid.";

		#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
		EXPECT_NE(sut.getThreadHandle(), nullptr) << "Windows thread native handle was not initialized.";
		#endif

		// Wait for the thread to finish
		sut.join();

		// The value should have changed
		EXPECT_TRUE(threadFunctionValue);
	}
	TEST(Threads, CanGetCurrentThread)
	{
		threadID id = thread::getCurrentThreadID();
		EXPECT_NE(id, threadID()) << "Current thread ID was null";

		thread sut;
		sut.initializeFromCurrentThread();

		EXPECT_EQ(id.m_ThreadID, sut.getThreadID()) << "Thread initialized from the current thread does not have the found current thread ID";
		sut.close();
	}
	
	static bool lambdaFunctionValue = false;
	TEST(Threads, CanBeCreatedWithLambda)
	{
		thread::threadCallback callbackFunction = [](thread*) { lambdaFunctionValue = true; return (unsigned int)0; };
		thread sut(callbackFunction);


		// Check that the thread was valid
		ASSERT_TRUE(sut.valid()) << "Initialzed thread was not valid.";

		#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
		EXPECT_NE(sut.getThreadHandle(), nullptr) << "Windows thread native handle was not initialized.";
		#endif

		// Wait for the thread to finish
		sut.join();

		// The value should have changed
		EXPECT_TRUE(lambdaFunctionValue);
	}

	static bool affinityFunctionValue = false;
	TEST(Threads, CanSetAffinity)
	{
		affinityFunctionValue = true;
		thread sut([](thread*) {while (affinityFunctionValue) {} return (unsigned int) 0; });

		// Check that the thread was valid
		ASSERT_TRUE(sut.valid()) << "Initialzed thread was not valid.";

		#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
		EXPECT_NE(sut.getThreadHandle(), nullptr) << "Windows thread native handle was not initialized.";
		#endif

		// Set the affinity
		size_t expectedAffinity = 1;
		sut.setAffinity(expectedAffinity);

		EXPECT_EQ(sut.getAffinity(), expectedAffinity);

		// End the loop
		affinityFunctionValue = false;
		sut.join();
	}

	static bool nameFunctionValue = false;
	TEST(Threads, CanSetName)
	{
		nameFunctionValue = true;
		thread sut([](thread*) {while (nameFunctionValue) {} return (unsigned int)0; });

		// Check that the thread was valid
		ASSERT_TRUE(sut.valid()) << "Initialzed thread was not valid.";

		#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
		EXPECT_NE(sut.getThreadHandle(), nullptr) << "Windows thread native handle was not initialized.";
		#endif

		// Set the affinity
		std::wstring expectedName = L"Test Name";
		sut.setName(expectedName);

		EXPECT_STREQ(sut.getName().c_str(), expectedName.c_str());

		// End the loop
		nameFunctionValue = false;
		sut.join();
	}

	static bool moveFunctionValue = false;
	TEST(Threads, CanBeMoveConstructed)
	{
		moveFunctionValue = true;
		thread thread1 = thread([](thread*) {while (moveFunctionValue) {} return (unsigned int)0; });
		thread sut = std::move(thread1);

		// Check that the thread was valid
		ASSERT_TRUE(sut.valid()) << "Initialzed thread was not valid.";

		#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
		EXPECT_NE(sut.getThreadHandle(), nullptr) << "Windows thread native handle was not initialized.";
		#endif

		moveFunctionValue = false;
		sut.join();
	}

	static bool detachFunctionValue = false;
	TEST(Threads, CanBeDetached)
	{
		thread sut([](thread*) {return (unsigned int)0; });

		// Check that the thread was valid
		ASSERT_TRUE(sut.valid()) << "Initialzed thread was not valid.";

		#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
		EXPECT_NE(sut.getThreadHandle(), nullptr) << "Windows thread native handle was not initialized.";
		#endif
		
		// Detach the thread
		sut.detach();


		// Check that the thread was valid
		EXPECT_FALSE(sut.valid()) << "Initialzed thread was not valid.";

		#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
		EXPECT_EQ(sut.getThreadHandle(), nullptr) << "Windows thread native handle was not initialized.";
		#endif

	}
}