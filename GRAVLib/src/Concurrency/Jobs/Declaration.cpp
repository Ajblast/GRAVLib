#include "GRAVLib/Concurrency/Jobs/Declaration.h"

GRAVLib::Concurrency::Jobs::declaration::declaration() :
	m_Arg(0), m_Priority(jobPriority::NORMAL)
{}
GRAVLib::Concurrency::Jobs::declaration::declaration(entryPoint entryPoint, uintptr_t arg, jobPriority priority) :
	m_EntryPoint(entryPoint), m_Arg(arg), m_Priority(priority)
{}
