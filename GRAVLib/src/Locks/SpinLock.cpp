#include "Locks/SpinLock.h"
#include <thread>

GRAVLib::Locks::spinLock::spinLock()
{

}
GRAVLib::Locks::spinLock::~spinLock()
{

}

bool GRAVLib::Locks::spinLock::try_lock()
{
    // Use an acquire fence to ensure all subsequent reads by this thread will be valid
    bool alreadyLocked = m_Atomic.test_and_set(std::memory_order_acquire);

    return !alreadyLocked;
}

void GRAVLib::Locks::spinLock::lock()
{
    // Spin until successful acquire
    while (!try_lock())
    {
        // Yield this thread
        std::this_thread::yield();
    }
}

void GRAVLib::Locks::spinLock::unlock()
{
    // Use release semantics to ensure that all prior write have been fully committed before unlocking
    m_Atomic.clear(std::memory_order_release);
}