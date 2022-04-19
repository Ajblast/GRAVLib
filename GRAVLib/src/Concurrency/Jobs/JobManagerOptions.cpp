#include "GRAVLib/Concurrency/Jobs/JobManagerOptions.h"
#include "GRAVLib/Concurrency/Threads/Thread.h"

GRAVLib::Concurrency::Jobs::jobManagerOptions::jobManagerOptions() :
	m_NumThreads(Threads::thread::getHardwareConcurrency())
{}
