#include "Concurrency/Jobs/JobManager.h"
#include "Debug/Instrumentation/Instrumentor.h"
#include "Concurrency/Fibers/Exceptions/FiberQuitCallbackException.h"
#include "Concurrency/Fibers/Exceptions/FiberCallbackException.h"
#include "Concurrency/Jobs/Exceptions/ReadyFiberException.h"
#include <algorithm>
#include <format>

GRAVLib_SINGLETON_SET_INSTANCE(GRAVLib::Concurrency::Jobs::jobManager);

GRAVLib::Concurrency::Jobs::jobManager::jobManager(jobManagerOptions& options) : singleton(),
	m_ThreadAffinity(options.m_ThreadAffinity),
	m_QueueBehavior(options.m_QueueBehavior),
	m_Callbacks(options.m_Callbacks)
{
	GRAVLib_PROFILE_FUNCTION();

	GRAV_ASSERT_TRUE(options.m_NumFibers > 0);
	GRAV_ASSERT_TRUE(options.m_NumThreads > 0);
	GRAV_ASSERT_TRUE(options.m_NumFibers >= options.m_NumThreads);
	if (options.m_ThreadAffinity)
		GRAV_ASSERT_TRUE(options.m_NumThreads <= Threads::thread::getHardwareConcurrency());
	GRAV_ASSERT_TRUE(options.m_MaxWaitingFibersCount > 1);

	GRAVLib_LOG_LINE_INFO("{}: Begin Job Manager Startup", GRAVLib_CLEAN_FUNC_SIG);

	GRAVLib_LOG_LINE_INFO("{}: Initialize Main Thread", GRAVLib_CLEAN_FUNC_SIG);

	#pragma region InitializeMainThread
	// Initialize thread
	scope<Threads::thread> mainThread = createScope<Threads::thread>();
	mainThread->initializeFromCurrentThread();
	m_MainThreadID = mainThread->getID();
	mainThread->setName(L"Main Thread");
	if (m_ThreadAffinity)
		mainThread->setAffinity(0);

	// Initialize the main thread fiber
	scope<Fibers::fiber> mainFiber = createScope<Fibers::fiber>();
	mainFiber->initializeFromCurrentThread();
	mainThread->getTLS()->m_Fiber = mainFiber->getID();
	mainThread->getTLS()->m_CurrentFiber = mainFiber->getID();
	m_Fibers.emplace(mainFiber->getID(), std::move(mainFiber));


	// Add the main thread to the thread map
	m_Threads.emplace(m_MainThreadID, std::move(mainThread));

	// Main thread on initialized callback
	if (m_Callbacks.m_OnThreadInitialized)
		m_Callbacks.m_OnThreadInitialized(m_MainThreadID);

	// Fiber attached callback for the main fiber because it wouldn't be called otherwise
	if (m_Callbacks.m_OnFiberAttached)
		m_Callbacks.m_OnFiberAttached(getThread(m_MainThreadID).getTLS()->m_CurrentFiber);
	#pragma endregion

	#pragma region SpawnFiberPool
	// Spawn the fiber pool
	{
		GRAVLib_PROFILE_SCOPE("Spawn Fibers");
		GRAVLib_LOG_LINE_INFO("{}: Spawn Fibers", GRAVLib_CLEAN_FUNC_SIG);

		// Spawn the other fibers
		for (Fibers::fiberIndex_t i = 0; i < options.m_NumFibers; i++)
		{
			GRAVLib_LOG_LINE_INFO("{}: Spawn Fiber", GRAVLib_CLEAN_FUNC_SIG);

			// Spawn a fiber for the fiber pool
			scope<Fibers::fiber> fiber = createScope<Fibers::fiber>(fiberCallback, std::to_string(i));

			// Add the fiber to the maps
			m_IdleFibers.emplace(fiber->getID(), true);
			m_Fibers.emplace(fiber->getID(), std::move(fiber));
		}

		// Fiber creation callback
		if (m_Callbacks.m_OnFibersCreated)
			m_Callbacks.m_OnFibersCreated(options.m_NumFibers);
	}
	#pragma endregion

	#pragma region SpawnWorkerThreads
	// Spawn the threads
	{
		GRAVLib_PROFILE_SCOPE("Spawn Worker Threads");
		GRAVLib_LOG_LINE_INFO("{}: Spawn Worker Threads", GRAVLib_CLEAN_FUNC_SIG);

		// Spawn the worker threads. Don't include the main thread
		for (Threads::threadIndex_t i = 1; i < options.m_NumThreads; i++)
		{
			GRAVLib_PROFILE_SCOPE("Spawn Worker Thread");

			scope<Threads::thread> thread;

			// Spawn a worker thread
			if (m_ThreadAffinity)
				thread = createScope<Threads::thread>(threadCallback, L"Worker Thread " + std::to_wstring(i), i);
			else
				thread = createScope<Threads::thread>(threadCallback, L"Worker Thread " + std::to_wstring(i));

			GRAVLib_LOG_LINE_INFO("{}: Spawn Worker Thread {}", GRAVLib_CLEAN_FUNC_SIG, *thread);

			// Add the worker thread to the map
			m_Threads.emplace(thread->getID(), std::move(thread));
		}

		// Thread creation callback
		if (m_Callbacks.m_OnThreadsCreated)
			m_Callbacks.m_OnThreadsCreated(options.m_NumThreads);
	}
	#pragma endregion


	m_IsValid.store(true, std::memory_order_relaxed);

	// Job manager statup callback
	if (m_Callbacks.m_OnJobManagerStartup)
		m_Callbacks.m_OnJobManagerStartup();

	GRAVLib_LOG_LINE_INFO("{}: End Startup", GRAVLib_CLEAN_FUNC_SIG);
}
GRAVLib::Concurrency::Jobs::jobManager::~jobManager() 
{
	GRAVLib_PROFILE_FUNCTION();
	GRAVLib_LOG_LINE_INFO("{}: Begin Shutdown", GRAVLib_CLEAN_FUNC_SIG);

	// The job manager is now shutting down
	m_ShuttingDown.store(true, std::memory_order_release);

	// Create the quit fibers
	for (auto it = m_Threads.begin(); it != m_Threads.end(); it++)
		m_ThreadQuitFibers.emplace(it->first, createScope<Fibers::fiber>(fiberQuitCallback, std::format("Quit Fiber Callback: {}", it->first)));


	// Jump to this thread's quit fiber. This is done just in case a different thread started the shutdown process.
	GRAVLib_LOG_LINE_INFO("{}: Jump to thread quit fiber to isolate the starting main thread", GRAVLib_CLEAN_FUNC_SIG);


	// Wake up all of the sleeping threads
	if (m_QueueBehavior.load(std::memory_order_relaxed) == queueBehavior::SLEEP)
		m_ThreadCV.notify_all();
	// Fiber detached callback
	if (m_Callbacks.m_OnFiberDetached)
		m_Callbacks.m_OnFiberDetached(getCurrentTLS()->m_CurrentFiber, false);

	// Switch the current fiber to the thread's quit fiber
	getFiber(getCurrentTLS()->m_CurrentFiber).switchTo(*m_ThreadQuitFibers[Threads::thread::getCurrentThreadID()]);



	// Fiber detached callback
	if (m_Callbacks.m_OnFiberAttached)
		m_Callbacks.m_OnFiberAttached(getCurrentTLS()->m_CurrentFiber);
	// This should be the main fiber again, so we can now wait for the other thread's to die.


	// Wait for all of the threads to complete now that the job manager is invalid
	GRAVLib_LOG_LINE_INFO("{}: Wait for worker threads to join", GRAVLib_CLEAN_FUNC_SIG);
	for (auto it = m_Threads.begin(); it != m_Threads.end(); it++)
	{
		// Close the thread fiber
		GRAVLib_LOG_LINE_INFO("{}: Convert the thread fiber [{}] back into a thread", GRAVLib_CLEAN_FUNC_SIG, *it->second);
		Fibers::fiber& threadFiber = getFiber(it->second->getTLS()->m_Fiber);
		Fibers::fiberID id = threadFiber.getID();
		threadFiber.convertToThread();
		m_Fibers.erase(id);

		// Join the thread unless its the main thread because the main thread detaches instead
		if (it->first != m_MainThreadID)
			it->second->join();
	}

	// Close all of the fibers
	for (auto it = m_Fibers.begin(); it != m_Fibers.end(); it++)
		it->second->close();
	for (auto it = m_ThreadQuitFibers.begin(); it != m_ThreadQuitFibers.end(); it++)
		it->second->close();

	// Destroy the main thread thread object
	getThread(m_MainThreadID).detach();

	if (m_Callbacks.m_OnJobManagerShutdown)
		m_Callbacks.m_OnJobManagerShutdown();

	// The job manager is now shutting down is invalid
	m_IsValid.store(false, std::memory_order_release);
	GRAVLib_LOG_LINE_INFO("{}: End Shutdown", GRAVLib_CLEAN_FUNC_SIG);
}


void GRAVLib::Concurrency::Jobs::jobManager::kickJob(const declaration& declaration)
{
	// Kick a job but don't associate it with a counter
	kickJob(declaration, nullptr);
}
void GRAVLib::Concurrency::Jobs::jobManager::kickJob(const declaration& declaration, ref<counter>* count)
{
	GRAVLib_PROFILE_FUNCTION();

	// Don't kick a job if the entry point is null
	if (declaration.m_EntryPoint == nullptr)
		return;

	//GRAVLib_LOG_LINE_INFO("{}: Kick Job | EntryPoint {}l | Priority {}", GRAVLib_CLEAN_FUNC_SIG, declaration.m_EntryPoint, toString(declaration.m_Priority));

	// Check if a memory address to store a potential counter exists
	if (count && *count)
		(*count)->increment();				// The memory address exists and the counter exists, so just increment the counter
	else if (count)
		(*count) = createRef<counter>(1);	// The memory address exists, but a counter doesn't, so make one

	// Enqeue the job
	getQueue(declaration.m_Priority).enqueue({ declaration, count });

	// Wake up one of the sleeping threads to do this job
	if (m_QueueBehavior.load(std::memory_order_relaxed) == queueBehavior::SLEEP)
		m_ThreadCV.notify_one();
}
void GRAVLib::Concurrency::Jobs::jobManager::kickJobs(const declaration* declarations, size_t count)
{
	GRAVLib_PROFILE_FUNCTION();

	// Kick an array of jobs not associated with a counter
	for (size_t i = 0; i < count; i++)
		kickJob(declarations[i]);
}
void GRAVLib::Concurrency::Jobs::jobManager::kickJobs(const scope<declaration[]>& declarations, size_t count)
{
	GRAVLib_PROFILE_FUNCTION();

	kickJobs(declarations.get(), count);
}
void GRAVLib::Concurrency::Jobs::jobManager::kickJobs(const declaration* declarations, size_t count, ref<counter>* counter)
{
	GRAVLib_PROFILE_FUNCTION();

	// Kick an array of jobs associated with a counter
	for (size_t i = 0; i < count; i++)
		kickJob(declarations[i], counter);
}
void GRAVLib::Concurrency::Jobs::jobManager::kickJobs(const scope<declaration[]>& declarations, size_t count, ref<counter>* counter)
{
	GRAVLib_PROFILE_FUNCTION();

	kickJobs(declarations.get(), count, counter);
}
void GRAVLib::Concurrency::Jobs::jobManager::waitForCounter(ref<counter>& counter, counterTarget target)
{
	GRAVLib_PROFILE_FUNCTION();

	// Check for premature finishing
	if (counter->getValue() == target)
	{
		// Make sure that the counter is not being actively used
		while (counter->m_DestructionLock.load() > 0) {}
		return;
	}

	// Get the thread local storage
	Threads::tls* tls = getCurrentTLS();
	Fibers::fiberID currentFiberID = tls->m_CurrentFiber;

	// Add the fiber to the waiting list, and check if it finished while doing so
	bool alreadyDone;
	try
	{
		alreadyDone = counter->addWaitingFiber(currentFiberID, target);
	}
	catch (...)
	{
		GRAVLib_LOG_LINE_CRITICAL("{}: Unable to switch to fiber!!!!", GRAVLib_CLEAN_FUNC_SIG);
		throw;
	}

	// Return if it finished already
	if (alreadyDone)
		return;

	// While waiting, run other jobs
	Fibers::fiberID freeFiberID = findFreeFiber();							// Find a free fiber

	// Update the thread local storage
	tls->m_CurrentFiber = freeFiberID;										// Find a free fiber

	// Switch to the new fiber from the current fiber
	switchToFiber(currentFiberID, freeFiberID, true);
}



void GRAVLib::Concurrency::Jobs::jobManager::addReadyFiber(Fibers::fiberID id)
{
	GRAVLib_PROFILE_FUNCTION();

	GRAVLib_LOCK_MUTEX(m_ReadyFiberLock);

	// Insert the new id to the end of the vector
	m_ReadyFibers.push_back(id);
}
GRAVLib::Concurrency::Fibers::fiberID GRAVLib::Concurrency::Jobs::jobManager::findReadyFiber()
{
	GRAVLib_LOCK_MUTEX(m_ReadyFiberLock);

	// Check if there is any ready fibers
	if (m_ReadyFibers.size() == 0)
		return Fibers::fiberID();

	// Get the ID and remove it from the list
	Fibers::fiberID id = m_ReadyFibers.front();
	m_ReadyFibers.erase(m_ReadyFibers.begin());

	return id;
}
GRAVLib::Concurrency::Fibers::fiberID GRAVLib::Concurrency::Jobs::jobManager::findFreeFiber()
{
	while (true)
	{
		// Scan through all of the fibers
		for (auto it = m_IdleFibers.begin(); it != m_IdleFibers.end(); it++)
		{
			// Check if the fiber is free
			if (it->second.load(std::memory_order_acquire) == false)
				continue;

			// See if the value is exchanged. If this fails, another thread beat this thread in exchanging, so don't double book the fiber
			bool expected = true;
			if (std::atomic_compare_exchange_strong_explicit(&it->second, &expected, false, std::memory_order_release, std::memory_order_acquire))
				return it->first;
		}
	}
}


GRAVLib::Concurrency::Jobs::jobQueue& GRAVLib::Concurrency::Jobs::jobManager::getQueue(jobPriority priority)
{
	switch (priority)
	{
	case GRAVLib::Concurrency::Jobs::jobPriority::LOW:
		return m_LowPriorityQueue;
		break;
	case GRAVLib::Concurrency::Jobs::jobPriority::NORMAL:
		return m_NormalPriorityQueue;
		break;
	case GRAVLib::Concurrency::Jobs::jobPriority::HIGH:
		return m_HighPriorityQueue;
		break;
	case GRAVLib::Concurrency::Jobs::jobPriority::CRITICAL:
		return m_CriticalPriorityQueue;
		break;
	default:
		return m_LowPriorityQueue;
		break;
	}
}
bool GRAVLib::Concurrency::Jobs::jobManager::getNextJob(job& job)
{
	// Always run critical jobs first
	if (m_CriticalPriorityQueue.dequeue(job))
		return true;
	if (m_HighPriorityQueue.dequeue(job))
		return true;
	if (m_NormalPriorityQueue.dequeue(job))
		return true;
	if (m_LowPriorityQueue.dequeue(job))
		return true;

	return false;
}

void GRAVLib::Concurrency::Jobs::jobManager::switchToFiber(Fibers::fiberID oldID, Fibers::fiberID newID, bool interrupted)
{
	if (m_Callbacks.m_OnFiberDetached)
		m_Callbacks.m_OnFiberDetached(oldID, interrupted);

	if (m_Callbacks.m_OnFiberAttached)
		m_Callbacks.m_OnFiberAttached(newID);

	// Switch to the new fiber
	getFiber(oldID).switchTo(getFiber(newID));
}

void GRAVLib::Concurrency::Jobs::jobManager::fiberCallback(Fibers::fiber* currentFiber)
{
	jobManager& instance = jobManager::getInstance();

	Fibers::fiberID id = currentFiber->getID();
	GRAVLib_LOG_LINE_INFO("{}: Start Fiber: {}", GRAVLib_CLEAN_FUNC_SIG, id);

	// Fiber initialized callback
	if (instance.m_Callbacks.m_OnFiberInitialized)
		instance.m_Callbacks.m_OnFiberInitialized(id);


	#pragma region FiberRunLoop
	job job;
	Fibers::fiberID readyID;
	while (instance.isShuttingDown() == false)
	{
		// Check if there are any fibers that are ready to be ran
		readyID = instance.findReadyFiber();
		if (readyID.valid())
		{
			// Ready fiber found
			instance.getCurrentTLS()->m_CurrentFiber = readyID;

			// This fiber is now idle and waiting for jobs
			instance.m_IdleFibers.at(id).store(true, std::memory_order_release);

			// Switch to the new fiber
			instance.switchToFiber(currentFiber->getID(), readyID, false);

			continue;
		}

		// Get the next job
		if (instance.getNextJob(job))
		{
			// Job Before callback
			if (instance.m_Callbacks.m_OnJobBefore)
				instance.m_Callbacks.m_OnJobBefore(job.m_Declaration);

			// Run the job
			job.m_Declaration.m_EntryPoint(job.m_Declaration.m_Arg);

			// Decrement the counter associated with this job if it exists
			if (job.m_Counter && *job.m_Counter)
				(*job.m_Counter)->decrement();

			// Job After callback
			if (instance.m_Callbacks.m_OnJobAfter)
				instance.m_Callbacks.m_OnJobAfter(job.m_Declaration);
		}
		else
		{
			// No job was found
			switch (instance.m_QueueBehavior)
			{
			case queueBehavior::YIELD:
				std::this_thread::yield();
				break;
			case queueBehavior::SLEEP:
			{
				// Lock the sleeping lock
				std::unique_lock<decltype(instance.m_SleepingThreadLock)> lock(instance.m_SleepingThreadLock);
				instance.m_ThreadCV.wait(lock);
				break;
			}
			case queueBehavior::SPIN:
				Threads::thread::sleepFor(1);
				break;
			default:
				break;
			}
		}
	}
	#pragma endregion


	// Fiber initialized callback
	if (instance.m_Callbacks.m_OnFiberEnds)
		instance.m_Callbacks.m_OnFiberEnds(id);

	GRAVLib_LOG_LINE_INFO("{}: End Fiber: {}", GRAVLib_CLEAN_FUNC_SIG, id);

	// Fiber detached callback. Has to be called explicitly because of switch to a thread quit fiber
	if (instance.m_Callbacks.m_OnFiberDetached)
		instance.m_Callbacks.m_OnFiberDetached(id, false);

	// Switch back to the calling fiber because the thread's have to die using their personal fibers
	currentFiber->switchTo(*instance.m_ThreadQuitFibers[instance.getCurrentThread().getID()]);

	// Fiber's switch to their quit equivalent fibers and should never be ran again. As such, this area should never be ran.
	throw Fibers::Exceptions::fiberCallbackException("Fiber code ran after switching to quit fiber. This code should never be reached!");
}
void GRAVLib::Concurrency::Jobs::jobManager::fiberQuitCallback(Fibers::fiber* fiber)
{
	jobManager& instance = jobManager::getInstance();

	// Wait for all of the threads to shut themselves down
	instance.m_ShutdownThreadCount.fetch_add(1, std::memory_order_seq_cst);
	while (instance.m_ShutdownThreadCount.load(std::memory_order_seq_cst) != instance.getNumThreads())
		Threads::thread::sleepFor(50);


	// Switch to the thread's personal fiber
	instance.m_ThreadQuitFibers[instance.getCurrentThread().getID()]->switchTo(instance.getFiber(instance.getCurrentTLS()->m_Fiber));

	// Fiber's switch to their quit equivalent fibers and should never be ran again. As such, this area should never be ran.
	throw Fibers::Exceptions::fiberQuitCallbackException("Fiber Quit Callback code ran after switching to threads's personal fiber. This code should never be reached!");
}
unsigned int GRAVLib::Concurrency::Jobs::jobManager::threadCallback(Threads::thread* thread)
{
	jobManager& instance = jobManager::getInstance();

	GRAVLib_LOG_LINE_INFO("{}: Initialize Thread: {}", GRAVLib_CLEAN_FUNC_SIG, thread->getID());

	// Wait until the job manager is valid
	while (instance.isValid() == false && instance.isShuttingDown() == false);
	{
		GRAVLib_LOG_LINE_INFO("{}: Thread: {} | Waiting for job manager to become valid.", GRAVLib_CLEAN_FUNC_SIG, thread->getID());
		Threads::thread::sleepFor(1);
	}

	// Get the thread's thread local storage
	Threads::tls* tls = thread->getTLS();

	// Thread initialized callback
	if (instance.m_Callbacks.m_OnThreadInitialized)
		instance.m_Callbacks.m_OnThreadInitialized(Threads::thread::getCurrentThreadID());


	// Create a new fiber for the thread and add it to the map
	scope<Fibers::fiber> threadFiber = createScope<Fibers::fiber>();
	threadFiber->initializeFromCurrentThread();
	tls->m_Fiber = threadFiber->getID();
	instance.m_Fibers.emplace(threadFiber->getID(), std::move(threadFiber));

	// Find a free fiber for this thread to start running
	Fibers::fiberID freeFiberIndex = instance.findFreeFiber();
	tls->m_CurrentFiber = freeFiberIndex;

	// Switch to the found fiber
	GRAVLib_LOG_LINE_INFO("{}: Thread: {} | Start fiber", GRAVLib_CLEAN_FUNC_SIG, thread->getID());
	instance.getFiber(tls->m_Fiber).switchTo(instance.getFiber(freeFiberIndex));
	GRAVLib_LOG_LINE_INFO("{}: Thread: {} | Thread Finished", GRAVLib_CLEAN_FUNC_SIG, thread->getID());

	// Fiber initialized callback
	if (instance.m_Callbacks.m_OnThreadEnds)
		instance.m_Callbacks.m_OnThreadEnds(Threads::thread::getCurrentThreadID());


	// Return 0 for success
	return 0;
}