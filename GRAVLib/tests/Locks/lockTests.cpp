#include <gtest/gtest.h>
#include "GRAVLibCore.h"
#include "Locks/SpinLock.h"
#include "Locks/MultiLock.h"

namespace GRAVLib::Locks
{
	TEST(SpinLock, CanOnlyBeLockedOnce)
	{
		spinLock sut = spinLock();

		bool firstLock = sut.try_lock();
		bool secondLock = sut.try_lock();

		EXPECT_TRUE(firstLock) << "Unable to lock the spin lock the first time.";
		EXPECT_FALSE(secondLock) << "Spin lock was lockable while it was already locked.";

		try
		{
			sut.unlock();
		}
		catch (...)
		{
			FAIL() << "Unable to unlock spin lock";
		}
	}

	TEST(MultiLock, CanLockLocks)
	{
		spinLock lock1 = spinLock();
		spinLock lock2 = spinLock();

		multiLock<spinLock, spinLock> sut = multiLock<spinLock, spinLock>(lock1, lock2);

		int firstLock = sut.try_lock();
		int secondLock = sut.try_lock();

		EXPECT_EQ(firstLock, -1) << "Unable to lock the multilock the first time.";
		EXPECT_EQ(secondLock, 0) << "Multilock was lockable while it was already locked.";

		try
		{
			sut.unlock();
		}
		catch (...)
		{
			FAIL() << "Unable to unlock multi lock";
		}

	}
}