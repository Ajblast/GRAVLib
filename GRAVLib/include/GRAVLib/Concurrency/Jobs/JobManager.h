#pragma once

#include "GRAVLib/GRAVLibCore.h"

#include "GRAVLib/Concurrency/Jobs/JobManagerOptions.h"
#include "GRAVLib/Concurrency/Jobs/Declaration.h"
#include "GRAVLib/Concurrency/Jobs/Counter.h"
#include "GRAVLib/Concurrency/Threads/Thread.h"
#include "GRAVLib/Concurrency/Fibers/Fiber.h"

#include "GRAVLib/Containers/ThreadSafe/Queue.h"
#include "GRAVLib/Concurrency/Locks/Locks.h"
#include "GRAVLib/Concurrency/Locks/SpinLock.h"
#include "GRAVLib/Memory/Singleton.h"

#include <unordered_map>
#include <vector>


// TODO: Implement I/O threads (Socks, file I/O, system call). Slide 19 on Parallelizing the Naughty Dog Engine Using Fibers
// TODO: Implement automatic allocation for counters

namespace GRAVLib::Concurrency::Jobs
{
	GRAVLib_JOB_ENTRY_POINT(switchToReadyFiber);

	struct GRAVLibAPI job
	{
		declaration m_Declaration;
		ref<counter>* m_Counter;
	};

	typedef Containers::ThreadSafe::queue<job> jobQueue;

	class GRAVLibAPI jobManager : public singleton<jobManager>
	{
		// The job manager is a friend of the counter so that waiting fibers can be added easierly and without complicating public API
		friend counter;
		// Switch to ready Fibers::fiber is a friend because it needs access to the fibers and the Threads::tls
		friend GRAVLib_JOB_ENTRY_POINT(switchToReadyFiber);

	public:
		jobManager();
		jobManager(jobManagerOptions& options);
		virtual ~jobManager();

		#pragma region KickJobs
		// Kick a single job
		void kickJob(const declaration& declaration);
		// Kick a single job and return a counter
		void kickJob(const declaration& declaration, ref<counter>* counter);
		// Kick an array of jobs. Jobs can be created on the stack as long as the array is not freed before all the jobs are ran
		void kickJobs(const declaration* declarations, size_t count);
		// Kick an array of jobs.
		void kickJobs(const scope<declaration[]>& declarations, size_t count);
		// Kick an array of jobs. Jobs can be created on the stack as long as the array is not freed before all the jobs are ran
		void kickJobs(const declaration* declarations, size_t count, ref<counter>* counter);
		// Kick an array of jobs.
		void kickJobs(const scope<declaration[]>& declarations, size_t count, ref<counter>* counter);
		// Wait for the atomic counter to be the target value before allowing execution to continue
		void waitForCounter(ref<counter>& counter, counterTarget target);
		#pragma endregion

		#pragma region Getters/Setters
		const uint8 getNumThreads() const;
		const uint16 getNumFibers() const;
		const bool isValid() const;
		const bool isShuttingDown() const;

		GRAVLib_SINGLETON_INSTANCE(jobManager);
		#pragma endregion
	private:
		void addReadyFiber(Fibers::fiberID id);

		#pragma region Getters
		Threads::tls* getCurrentTLS() const;
		Threads::thread& getThread(Threads::threadID id) const;
		Threads::thread& getCurrentThread() const;

		Fibers::fiber& getIdleFiber(Fibers::fiberID id) const;
		Fibers::fiber& getFiber(Fibers::fiberID id) const;
		Fibers::fiber& getCurrentFiber() const;
		#pragma endregion


		Fibers::fiberID findReadyFiber();
		Fibers::fiberID findFreeFiber();

		// Get a jobQueue by priority. Defaults to low priority queue
		jobQueue& getQueue(jobPriority priority);
		// Get the next job
		bool getNextJob(job& job);

		void switchToFiber(Fibers::fiberID oldID, Fibers::fiberID newID, bool interrupted = false);

		#pragma region Callbacks
		static void fiberCallback(Fibers::fiber*);
		static void fiberQuitCallback(Fibers::fiber*);
		static unsigned int threadCallback(Threads::thread*);
		#pragma endregion
	private:
		// Threads
		bool m_ThreadAffinity;															// Should threads be set with a thread affinity
		std::unordered_map<Threads::threadID, scope<Threads::thread>> m_Threads;		// The threads
		std::unordered_map<Threads::threadID, scope<Fibers::fiber>> m_ThreadQuitFibers; // The fibers that are explicitly used to make sure that threads switch back to their personal fibers
		Threads::threadID m_MainThreadID;

		// Fibers
		std::unordered_map<Fibers::fiberID, scope<Fibers::fiber>> m_Fibers;				// Map from fiberID to fiber
		std::unordered_map<Fibers::fiberID, std::atomic_bool> m_IdleFibers;				// Map that tells which fibers are idle and free
		std::vector<Fibers::fiberID> m_ReadyFibers;
		Locks::spinLock m_ReadyFiberLock;

		// Thread shutdown infomation
		std::atomic_size_t m_ShutdownThreadCount;	// The amount of threads that have shutdown and finished

		// Thead sleeping behavior
		std::mutex m_SleepingThreadLock;			// Lock used for when threads sleep instead of spin locking
		std::condition_variable m_ThreadCV;			// Condition variable to wake up worker threads when they sleep instead of spin locking
		std::atomic<queueBehavior> m_QueueBehavior;	// What is the thread sleeping behavior when there are no jobs


		// Job queues
		jobQueue m_CriticalPriorityQueue;			// The critical priority job queue
		jobQueue m_HighPriorityQueue;				// The high priority job queue
		jobQueue m_NormalPriorityQueue;				// The normal priority job queue
		jobQueue m_LowPriorityQueue;				// The low priority job queue

		jobManagerCallbacks m_Callbacks;			// Callbacks for the job manager

		std::atomic_bool m_IsValid = false;			// Is the job manager valid?
		std::atomic_bool m_ShuttingDown = false;	// Is the job manager valid?
	};

	inline const uint8 jobManager::getNumThreads() const { return m_Threads.size(); }
	inline const uint16 jobManager::getNumFibers() const { return m_Fibers.size(); }
	inline const bool jobManager::isValid() const { return m_IsValid.load(std::memory_order_acquire); }
	inline const bool jobManager::isShuttingDown() const { return m_ShuttingDown.load(std::memory_order_acquire); }

	inline Threads::tls* jobManager::getCurrentTLS() const
	{
		return getCurrentThread().getTLS();
	}
	inline Threads::thread& jobManager::getThread(Threads::threadID id) const
	{
		const scope<Threads::thread>& thread = m_Threads.at(id);
		return *thread;
	}
	inline Threads::thread& jobManager::getCurrentThread() const
	{
		return getThread(Threads::thread::getCurrentThreadID());
	}
	inline Fibers::fiber& jobManager::getFiber(Fibers::fiberID id) const
	{
		const scope<Fibers::fiber>& fiber = m_Fibers.at(id);
		return *fiber;
	}
	inline Fibers::fiber& jobManager::getCurrentFiber() const
	{
		return getFiber(Fibers::fiber::getCurrenterFiberID());
	}
}