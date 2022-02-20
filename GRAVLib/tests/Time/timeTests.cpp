#include <gtest/gtest.h>
#include <thread>
#include "GRAVLibCore.h"
#include "Time/Stopwatch.h"

namespace GRAVLib::Time
{
	TEST(Stopwatch, HasZeroDeltaTimeWhenInitialized)
	{
		stopwatch sut = stopwatch();

		timeDurationCount count = sut.deltaTime().nanoseconds();

		ASSERT_EQ(count, 0) << "Stopwatch deltatime is not zero when initialized.";
		ASSERT_FALSE(sut.isRunning()) << "Stopwatch is running when initialized.";
	}

	TEST(Stopwatch, HasGreaterThanZeroDeltaTimeWhenStarted)
	{
		stopwatch sut = stopwatch();
		sut.start();

		EXPECT_TRUE(sut.isRunning()) << "Stopwatch is not running after starting.";

		std::this_thread::sleep_for(Time::milliseconds(10));

		timeDurationCount count = sut.deltaTime().nanoseconds();

		EXPECT_TRUE(sut.isRunning()) << "Stopwatch is not running after starting.";
		ASSERT_TRUE(count > 0) << "Stopwatch deltatime is not greater than zero after running.";
	}

	TEST(Stopwatch, HasSameDeltaTimeWhenPaused)
	{
		stopwatch sut = stopwatch();
		sut.start();

		EXPECT_TRUE(sut.isRunning()) << "Stopwatch is not running after starting.";

		std::this_thread::sleep_for(Time::milliseconds(3));
		sut.pause();

		ASSERT_FALSE(sut.isRunning()) << "Stopwatch is running while paused.";

		timeDurationCount count = sut.deltaTime().nanoseconds();
		std::this_thread::sleep_for(Time::milliseconds(3));

		ASSERT_FALSE(sut.isRunning()) << "Stopwatch is running when paused.";
		timeDurationCount count2 = sut.deltaTime().nanoseconds();

		ASSERT_EQ(count, count2) << "Stopwatch deltatime is the same after pausing and sleeping.";
	}

	TEST(Stopwatch, HasZeroDeltaTimeWhenReset)
	{
		stopwatch sut = stopwatch();
		sut.start();
		
		EXPECT_TRUE(sut.isRunning()) << "Stopwatch is not running after starting.";
		std::this_thread::sleep_for(Time::milliseconds(3));
		sut.pause();

		timeDurationCount count = sut.deltaTime().nanoseconds();
		EXPECT_TRUE(count > 0) << "Stopwatch deltatime was not zero after running and pausing";

		sut.reset();

		ASSERT_EQ(sut.deltaTime().nanoseconds(), 0) << "Stopwatch deltatime was not reset to 0.";
		ASSERT_FALSE(sut.isRunning()) << "Stopwatch is running after being reset.";
	}


}
