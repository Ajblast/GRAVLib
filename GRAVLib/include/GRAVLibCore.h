#pragma once

#include "Platform.h"

#ifdef GRAVLib_EXPORTS
	#define GRAVLibAPI __declspec(dllexport)
#else
	#define GRAVLibAPI __declspec(dllimport)
#endif

#include "Primitives.h"
#include "Memory/Pointers.h"