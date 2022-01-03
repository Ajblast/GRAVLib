#include <gtest/gtest.h>
#include "GRAVLibCore.h"
#include "Events/Event.h"
#include <functional>

namespace GRAVLib::Events
{
	class GRAVLibAPI simpleEventHandleGenerator : public IEventHandleGenerator
	{
	public:
		virtual ~simpleEventHandleGenerator() {}

		virtual eventHandle generate() override
		{
			return m_CurrentHandle++;
		}
	private:
		eventHandle m_CurrentHandle = 0;
	};


	class EventTest : public ::testing::Test {
	public:
		EventTest() 
		{
		}
		~EventTest() override
		{
		}
	protected:
		void SetUp() override
		{
			handleGenerator = createRef<simpleEventHandleGenerator>();
		}
		void TearDown() override 
		{
		}

		bool memberFunction(bool handled)
		{
			return true;
		}

		ref<IEventHandleGenerator> handleGenerator;
	};
	TEST_F(EventTest, CanRegisterLambda)
	{
		event sut = event(handleGenerator);

		try
		{
			eventHandle handle = sut.registerCallback([](bool handled) {return true; });

			EXPECT_EQ(handle, 0) << "First simple handle is not 0";
			EXPECT_EQ(sut.size(), 1) << "Event size is not 1 with a single registered callback";
			EXPECT_TRUE(sut.execute()) << "Simple callback was not handled";
		}
		catch (...)
		{
			EXPECT_TRUE(false) << "Exception occured while registering event";
		}
	}

	bool freeFunction(bool handled) { return true; }
	TEST_F(EventTest, CanRegisterFreeFunction)
	{
		event sut = event(handleGenerator);

		try
		{
			eventHandle handle = sut.registerCallback(freeFunction);

			EXPECT_EQ(handle, 0) << "First simple handle is not 0";
			EXPECT_EQ(sut.size(), 1) << "Event size is not 1 with a single registered callback";
			EXPECT_TRUE(sut.execute()) << "Simple callback was not handled";
		}
		catch (...)
		{
			EXPECT_TRUE(false) << "Exception occured while registering event";
		}
	}

	class simpleClass
	{
	public:
		bool memberFunction(bool handled) { return true; }
	};
	TEST_F(EventTest, CanRegisterMemberFunction)
	{
		event sut = event(handleGenerator);

		try
		{
			eventHandle handle = sut.registerCallback(GRAV_BIND_EVENT_FN(memberFunction));

			EXPECT_EQ(handle, 0) << "First simple handle is not 0";
			EXPECT_EQ(sut.size(), 1) << "Event size is not 1 with a single registered callback";
			EXPECT_TRUE(sut.execute()) << "Simple callback was not handled";
		}
		catch (...)
		{
			EXPECT_TRUE(false) << "Exception occured while registering event";
		}
	}
}