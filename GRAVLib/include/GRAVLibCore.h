#pragma once

#ifdef GRAVLib_EXPORTS
	#define GRAVLibAPI __declspec(dllexport)
#else
	#define GRAVLibAPI __declspec(dllimport)
#endif


#include "Platform.h"
#include "Primitives.h"
#include "Concurrency/Locks/Locks.h"
#include "Utils/MacroDefinitions.h"
#include "Utils/FunctionSignature.h"
#include "Memory/Pointers.h"
#include "Debug/Assertions.h"
//#include "Debug/Logging/Logging.h"