#include "Concurrency/Jobs/JobManagerOptions.h"
#include "Concurrency/Threads/Thread.h"

GRAVLib::Concurrency::Jobs::jobManagerOptions::jobManagerOptions() :
	m_NumThreads(Threads::thread::getHardwareConcurrency())
{}
