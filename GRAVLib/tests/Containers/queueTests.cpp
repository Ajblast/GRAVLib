#include <gtest/gtest.h>
#include "GRAVLibCore.h"
#include "Containers/ThreadSafe/Queue.h"

namespace GRAVLib::Containers::ThreadSafe
{
	// Can initialize
	TEST(Queue, InitialzesAsEmpty)
	{
		queue<int> sut = queue<int>();

		EXPECT_TRUE(sut.empty()) << "Empty queue was not empty.";
		EXPECT_EQ(sut.size(), 0) << "Empty queue did not have a size of 0.";
	}
	TEST(Queue, CanCopyConstruct)
	{
		queue<int> sut = queue<int>();
		sut.enqueue(3);

		queue<int> sut2 = queue<int>(sut);
		int data;
		int data2;

		EXPECT_EQ(sut.size(), sut2.size()) << "Copy constructed queue did not have the same size as the original.";

		sut.dequeue(data);
		sut2.dequeue(data2);
		EXPECT_EQ(data, data2) << "Copy constructed queue did not have the same data item.";
	}
	TEST(Queue, CanMoveConstruct)
	{
		int value = 3;
		queue<int> sut = queue<int>();
		sut.enqueue(value);

		queue<int> sut2 = queue<int>(std::move(sut));
		int data;

		EXPECT_TRUE(sut.empty()) << "Moved queue was not empty.";
		EXPECT_EQ(sut.size(), 0) << "Moved queue hand data items.";

		EXPECT_FALSE(sut2.empty()) << "Move constructed queue was empty.";
		EXPECT_EQ(sut2.size(), 1) << "Move constructed queue did not have a single item";
		sut2.dequeue(data);
		EXPECT_EQ(data, value) << "Move constructed data was not the same.";
	}


	// Enqueue copy construct
	TEST(Queue, CanEnqueueCopyConstruct)
	{
		queue<int> sut = queue<int>();
		sut.enqueue(3);

		EXPECT_FALSE(sut.empty()) << "Copy constructed enqueue was empty.";
		EXPECT_EQ(sut.size(), 1) << "Copy constructed enqueue did not have a single item.";
	}
	// Enqueue move onstruct
	TEST(Queue, CanEnqueueMoveConstruct)
	{
		queue<int> sut = queue<int>();
		sut.enqueue(std::move(3));

		EXPECT_FALSE(sut.empty()) << "Move constructed enqueue was empty.";
		EXPECT_EQ(sut.size(), 1) << "Move constructed enqueue did not have a single item.";
	}
	// Can dequeue
	TEST(Queue, CanDequeItems)
	{
		int value = 3;
		queue<int> sut = queue<int>();
		sut.enqueue(value);
		int data;

		sut.dequeue(data);
		EXPECT_EQ(data, value) << "Dequeued data was not the same as the original.";
	}


	class TwoQueues : public ::testing::Test {
	public:
		TwoQueues()
		{
		}
		~TwoQueues() override
		{
		}
	protected:
		void SetUp() override
		{
			sut1.enqueue(1);
			sut1.enqueue(2);
			sut1.enqueue(3);

			sut2.enqueue(1);
			sut2.enqueue(2);
			sut2.enqueue(3);
		}
		void TearDown() override
		{
		}

		queue<int> sut1;
		queue<int> sut2;
	};

	TEST_F(TwoQueues, WillBeEqual)
	{
		EXPECT_EQ(sut1, sut2) << "Queues are not equal.";
	}
	TEST_F(TwoQueues, WillNotBeEqual)
	{
		sut2.enqueue(4);
		EXPECT_NE(sut1, sut2) << "Queues are equal.";
	}

	TEST_F(TwoQueues, CanBeLessThan)
	{
		FAIL();
	}
	TEST_F(TwoQueues, CanBeLessThanEqual)
	{
		FAIL();
	}
	TEST_F(TwoQueues, CanBeGreaterThan)
	{
		FAIL();
	}
	TEST_F(TwoQueues, CanBeGreaterThanEqual)
	{
		FAIL();
	}
}