#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Fibers/Fiber.h"
#include <atomic>

#define MAX_NUM_WAITING_FIBERS 4

namespace GRAVLib::Concurrency::Jobs
{
	typedef uint32 counterTarget;	// A target for a counter

	// Forward define the job manager so that we can store a pointer to it
	class jobManager;

	class GRAVLibAPI counter
	{
		friend jobManager;	// Job manager is a friend to make it easier to add a waiting fiber

	public:
		counter(counterTarget initialValue, Fibers::fiberIndex_t maxWaitingFibers = MAX_NUM_WAITING_FIBERS);
		counter(const counter& other) = delete;					// Not allowed to copy construct
		counter(counter&& other) noexcept = delete;				// Not allowed to move construct
		counter& operator= (const counter& other) = delete;		// Not allowed to copy set
		counter& operator= (counter&& other) noexcept = delete;	// Not allowed to move set
		~counter();

		// Atomically increment the counter
		counterTarget increment();
		// Atomically decrement the counter
		counterTarget decrement();
		// Atomically get the value in the counter
		counterTarget getValue();

	private:
		// A slot in the waiting fiber array
		struct waitingFiberSlot
		{
			Fibers::fiberID m_ID;				// The fiberID
			counterTarget m_TargetValue = 0;	// The target value the fiber is waiting for
		};

		// Add a fiber to this counter's waiting list. Returns whether the waiting was already done while add this fiber to the waiting list
		bool addWaitingFiber(Fibers::fiberID id, counterTarget target);
		// Check if there are any fiber in the waiting list that need to be brought out of the list and be ran because their target == value
		void checkWaitingFibers(counterTarget value);

	private:
		std::atomic<counterTarget> m_Counter = 0;		// Counter starts with 0, but will be initialized to the initial value
		std::atomic_uint16_t m_DestructionLock = 0;		// A single atomic counter that makes sure that the counter can't be destructed while incrementing/decrementing

		Fibers::fiberIndex_t m_FiberCount;				// The maximum number of fibers for waiting
		scope<std::atomic_bool[]> m_FreeFibers;			// Which fibers are free
		scope<waitingFiberSlot[]> m_WaitingFibers;		// The slots where a fiber is placed when it needs to wait
		scope<std::atomic_bool[]> m_WaitingFiberInUse;	// An array holding which waiting fiber is being used to add/remove data.
	};
}