#include "GRAVLibCore.h"
#include "Concurrency/Jobs/JobManager.h"
#include "Concurrency/Jobs/JobSystem.h"
#include "Time/Stopwatch.h"
#include <gtest/gtest.h>
#include <format>
#include <iostream>

namespace GRAVLib::Concurrency::Jobs
{
	class JobManager : public ::testing::Test {
	public:
		JobManager()
		{
		}
		~JobManager() override
		{
		}
	protected:
		void SetUp() override
		{
			m_DummyJobCount.store(0, std::memory_order_release);

			// Manually check the job manager to make sure it isn't valid
			ASSERT_EQ(jobManager::getInstancePtr(), nullptr) << "Job manager was valid while setting up test.";

			jobManagerOptions options;
			options.m_NumThreads = m_ThreadCount;
			options.m_NumFibers = m_FiberCount;

			m_SUT = createScope<jobManager>(options);

			// Assert that the job manager was created properly
			ASSERT_TRUE(m_SUT->isValid()) << "Job manager was not valid after initialization.";
			ASSERT_FALSE(m_SUT->isShuttingDown()) << "Job manager was shutting down immediately after initialization.";
			ASSERT_NE(m_SUT->getInstancePtr(), nullptr) << "Job manager pointer was null after initialization.";
			EXPECT_EQ(m_SUT->getNumThreads(), m_ThreadCount) << "Job manager thread count was not set properly.";
			EXPECT_EQ(m_SUT->getNumFibers(), m_FiberCount + m_ThreadCount)  << "Job manager fiber count was not set properly.";
		}
		void TearDown() override
		{
			// Reset the job manager to a new state
			m_SUT.reset();

			// Manually check the job manager to make sure it has shutdown
			EXPECT_EQ(jobManager::getInstancePtr(), nullptr) << "Job manager instance was still set after destructing.";
			if (jobManager::getInstancePtr())
				EXPECT_FALSE(jobManager::getInstance().isValid()) << "While expecting the job manager to have a null pointer, it also was still valid.";

		}

		GRAVLib_JOB_ENTRY_POINT(dummyWaitJob)
		{
			Threads::thread::sleepFor(m_WaitTime);

			m_DummyJobCount.fetch_add(1, std::memory_order_acq_rel);
		}

		std::atomic<int> m_DummyJobCount = 0;

		scope<jobManager> m_SUT;
		Threads::threadIndex_t m_ThreadCount = 1;
		Fibers::fiberIndex_t m_FiberCount = 64;
		size_t m_JobCount = 10;
		size_t m_WaitTime = 3;
	};

	TEST_F(JobManager, CanWaitForJob)
	{
		Time::stopwatch sp;

		declaration job = { GRAVLib_BIND_FN(dummyWaitJob), 0, jobPriority::NORMAL };
		ref<counter> counter;
		
		sp.start();
		GRAVLib_KICK_JOB(job, &counter);
		EXPECT_EQ(counter->getValue(), 1) << "Job counter not equal to number of jobs.";

		GRAVLib_WAIT_COUNTER(counter, 0);
		sp.pause();

		// Wait job should be 5ms or more
		EXPECT_TRUE(sp.deltaTime().milliseconds() >= m_WaitTime) << "Waiting for the job didn't take enough time.";
		EXPECT_EQ(m_DummyJobCount.load(std::memory_order_acquire), 1) << "Amount of finished jobs didn't match job count.";
	}
	TEST_F(JobManager, CanWaitForJobs)
	{
		Time::stopwatch sp;

		scope<declaration[]> jobs = createScope<declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { GRAVLib_BIND_FN(dummyWaitJob), 0, jobPriority::NORMAL };
		ref<counter> counter;

		sp.start();
		GRAVLib_KICK_JOBS(jobs, m_JobCount, &counter);
		EXPECT_EQ(counter->getValue(), m_JobCount) << "Job counter not equal to number of jobs.";

		GRAVLib_WAIT_COUNTER(counter, 0);
		sp.pause();

		// Wait job should be 5ms or more per job
		EXPECT_TRUE(sp.deltaTime().milliseconds() >= m_WaitTime * m_JobCount) << "Waiting for the job didn't take enough time.";
		EXPECT_EQ(m_DummyJobCount.load(std::memory_order_acquire), m_JobCount) << "Amount of finished jobs didn't match job count.";
	}
	TEST_F(JobManager, CanWaitForMultipleCounters) 
	{
		Time::stopwatch sp;

		scope<declaration[]> jobs = createScope<declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { GRAVLib_BIND_FN(dummyWaitJob), 0, jobPriority::NORMAL };

		ref<counter> counter1;
		ref<counter> counter2;

		sp.start();
		GRAVLib_KICK_JOBS(jobs, m_JobCount, &counter1);
		GRAVLib_KICK_JOBS(jobs, m_JobCount, &counter2);
		EXPECT_EQ(counter1->getValue(), m_JobCount) << "Job counter not equal to number of jobs.";
		EXPECT_EQ(counter2->getValue(), m_JobCount) << "Job counter not equal to number of jobs.";

		GRAVLib_WAIT_COUNTER(counter1, 0);
		GRAVLib_WAIT_COUNTER(counter2, 0);
		sp.pause();

		// Wait job should be 5ms or more per job
		EXPECT_TRUE(sp.deltaTime().milliseconds() >= m_WaitTime * m_JobCount) << "Waiting for the job didn't take enough time.";
		EXPECT_EQ(m_DummyJobCount.load(std::memory_order_acquire), m_JobCount * 2) << "Amount of finished jobs didn't match job count.";
	}

	// TODO: JobManagerCallback Tests
}