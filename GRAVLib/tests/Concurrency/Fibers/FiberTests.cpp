#include <gtest/gtest.h>
#include "GRAVLibCore.h"
#include "Concurrency/Fibers/Fiber.h"

namespace GRAVLib::Concurrency::Fibers
{
	TEST(Fibers, CanBeInitializedFromCurrentThread)
	{
		fiber sut;
		sut.initializeFromCurrentThread();

		ASSERT_TRUE(sut.valid()) << "Fiber initialized from current thread was not valid.";

		// Have to turn the tread back into
		sut.convertToThread();

		EXPECT_FALSE(sut.valid()) << "Fiber initialized from current thread was still valid after converting back to thread.";
	}
	TEST(Fibers, CanBeCreated)
	{
		fiber sut = fiber([](fiber*) {});


		// Close the created fiber
		sut.close();
	}


	static fiber* sutPTR = nullptr;
	static bool callbackRan = false;
	static void testCallback(fiber* fiber)
	{
		// The callback was ran.
		callbackRan = true;
		// Switch back to the fiber
		fiber->switchTo(*sutPTR);
	}

	TEST(Fibers, CanSwitchToOtherFibers)
	{
		// Initialize the current fiber so switching can occur
		fiber sut;
		sut.initializeFromCurrentThread();
		sutPTR = &sut;
		callbackRan = false;

		ASSERT_TRUE(sut.valid()) << "The starting fiber is not valid.";

		// Create the second fiber
		fiber sut2 = fiber(&testCallback);
		ASSERT_TRUE(sut2.valid()) << "The second fiber is not valid.";

		// Call the other sut
		sut.switchTo(sut2);

		// Turn fiber back to thread for cleaning things up
		sut.convertToThread();
		// Close the second fiber
		sut2.close();


		EXPECT_FALSE(sut.valid()) << "Starting fiber is still valid after converting to thread.";
		EXPECT_FALSE(sut2.valid()) << "Second fiber is still valid after closing.";
		EXPECT_TRUE(callbackRan) << "Callback function for fiber was not run.";
	}
	TEST(Fibers, CanSetName)
	{
		std::string testName = "TestName";
		fiber sut;

		sut.setName(testName);

		EXPECT_STREQ(sut.name().c_str(), testName.c_str()) << "Fiber name was not set.";
	}
	TEST(Fibers, CanGetCurrentFiber)
	{
		fiber sut;
		sut.initializeFromCurrentThread();

		fiberID id = fiber::getCurrenterFiberID();
		EXPECT_EQ(sut.getID(), id) << "Current fiber ID does not match the created fiber ID.";
		sut.convertToThread();
	}
}