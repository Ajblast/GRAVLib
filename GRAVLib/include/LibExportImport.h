#pragma once

#ifdef GRAVLib_EXPORTS
	#define GRAVLibAPI __declspec(dllexport)
#else
	#define GRAVLibAPI __declspec(dllimport)
#endif
