#define GRAVLib_PLATFORM_UNKNOWN		0
#define GRAVLib_PLATFORM_WINDOWS		1
#define GRAVLib_PLATFORM_LINUX			2
#define GRAVLib_PLATFORM_APPLE			3
#define GRAVLib_PLATFORM_IOS			4
#define GRAVLib_PLATFORM_ANDROID		5

#define GRAVLib_COMPILER_UNKNOWN		0
#define GRAVLib_COMPILER_MSVC			1
#define GRAVLib_COMPILER_GNU			2
#define GRAVLib_COMPILER_CLANG			3

#define GRAVLib_ARCHITECTURE_32			1
#define GRAVLib_ARCHITECTURE_64			2

#pragma region Compiler
// Find the compiler

#if defined( _MSC_VER )
	#define GRAVLib_COMPILER GRAVLib_COMPILER_MSVC
	#define GRAVLib_COMP_VER _MSC_VER
#elif defined( __clang__ )
	#define GRAVLib_COMPILER GRAVLib_COMPILER_CLANG
	#define GRAVLib_COMP_VER (((__clang_major__)*100) + \
		    (__clang_minor__*10) + \
			__clang_patchlevel__)
#elif defined( __GNUC__ )
	#define GRAVLib_COMPILER GRAVLib_COMPILER_GNU
	#define GRAVLib_COMP_VER (((__GNUC__)*100) + \
			(__GNUC_MINOR__*10) + \
			__GNUC_PATCHLEVEL__)
#else
	#define GRAVLib_COMPILER GRAVLib_COMPILER_UNKNOWN
	#error "Unknown Compiler"
#endif
#pragma endregion

#pragma region Platform
// Platform detection using predefined macros

#ifdef _WIN32
	/* Windows x64/x86 */
	#define GRAVLib_PLATFORM GRAVLib_PLATFORM_WINDOWS
	
	#ifdef _WIN64
		/* Windows x64  */
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif

	// Disable CRT Warnings
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	* so we must check all of them (in this order)
	* to ensure that we're running on MAC
	* and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#define GRAVLib_PLATFORM GRAVLib_PLATFORM_IOS
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define GRAVLib_PLATFORM GRAVLib_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define GRAVLib_PLATFORM_APPLE
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
		#endif
/* We also have to check __ANDROID__ before __linux__
* since android is based on the linux kernel
* it has __linux__ defined */
#elif defined(__ANDROID__)
	#define GRAVLib_PLATFORM GRAVLib_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define GRAVLib_PLATFORM GRAVLib_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#define GRAVLib_PLATFORM GRAVLib_PLATFORM_UNKNOWN
	#error "Unknown platform!"
#endif
#pragma endregion

#pragma region Architecture
// Architecture Detection

#if defined(__LP64__) || defined(_WIN64)
#   define GRAVLib_ARCHITECTURE GRAVLib_ARCHITECTURE_64
#else
#   define GRAVLib_ARCHITECTURE GRAVLib_ARCHITECTURE_32
#endif
#pragma endregion

#pragma region Warnings
#define GRAVLib_QUOTE_INPLACE(x) # x
#define GRAVLib_QUOTE(x) GRAVLib_QUOTE_INPLACE(x)
// Compiler time warning. Must be preceded with #
#define GRAVLib_WARN( x )  pragma message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )
#pragma endregion