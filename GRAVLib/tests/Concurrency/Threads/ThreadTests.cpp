#include <gtest/gtest.h>
#include "GRAVLibCore.h"

namespace GRAVLib::Concurrency::Threads
{

}


// TODO: Thread Tests


// Test the formatting of a thread ID

// Can initialze from current thread
// Can be created
// Can set affinity
// Can set name


// Following code is old tests. Base tests on it

//void GRAVCore::Jobs::gravThread::canInitializeFromCurrentThread()
//{
//	GRAVEngine::Jobs::gravThread sut;
//
//	sut.initializeFromCurrentThread();
//
//	Assert::IsTrue(sut.isValid());
//}
//
//void GRAVCore::Jobs::gravThread::canBeCreated()
//{
//	GRAVEngine::Jobs::gravThread sut;
//
//	sut.spawn([](GRAVEngine::Jobs::gravThread*) {});
//
//	Assert::IsTrue(sut.isValid());
//}
//
//static bool continueWhileThread = true;
//void GRAVCore::Jobs::gravThread::canSetAffinity()
//{
//	GRAVEngine::Jobs::gravThread sut;
//
//	// Create a stupid thread
//	sut.spawn([](GRAVEngine::Jobs::gravThread*) { while (continueWhileThread) {}});
//
//	size_t affinity = 1;
//	sut.setAffinity(affinity);
//
//	size_t threadAffinity = sut.getAffinity();
//
//	continueWhileThread = false;
//	sut.join();
//
//	Assert::IsTrue(sut.isValid() && threadAffinity == affinity);
//}
//
//void GRAVCore::Jobs::gravThread::canSetName()
//{
//	GRAVEngine::Jobs::gravThread sut;
//
//	sut.initializeFromCurrentThread();
//	std::wstring name = L"Test Thread Name";
//	sut.setName(name);
//
//	std::wstring threadName = sut.getName();
//	bool compare = threadName.compare(name) == 0;
//	Assert::IsTrue(sut.isValid() && compare);
//}