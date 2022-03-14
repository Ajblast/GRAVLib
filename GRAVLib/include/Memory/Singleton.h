#pragma once

#include "GRAVLibCore.h"

#if GRAVLib_COMPILER == GRAVLib_COMPILER_MSVC
#   pragma warning (push)
#   pragma warning ( disable: 4661)
#endif


#define GRAVLib_SINGLETON_INSTANCE(singletonType) \
	static singletonType& getInstance() { GRAV_ASSERT_NE(s_Instance, nullptr); return *s_Instance; } \
	static singletonType* getInstancePtr() {return s_Instance;}

#define GRAVLib_SINGLETON_SET_INSTANCE(singletonType) template<> singletonType* GRAVLib::singleton<singletonType>::s_Instance = nullptr


namespace GRAVLib
{
	// A singleton is any object that should only every have a single instance. All singletons must implement GRAVLib_SINGLETON_INSTANCE
	template<typename T>
	class singleton
	{
	public:
		singleton();
		singleton(const singleton<T>& other) = delete;
		singleton(singleton<T>&& other) = delete;
		singleton& operator=(const singleton<T>& other) = delete;
		singleton& operator=(singleton<T>&& other) = delete;
		virtual ~singleton();

		// Get singleton instance. Must be explicitly declared in derived classes through GRAVLib_SINGLETON_INSTANCE
		static T& getInstance();
		// Get singleton instance. Must be explicitly declared in derived classes
		static T* getInstancePtr();
	protected:
		static T* s_Instance;
	};


	template<typename T>
	inline singleton<T>::singleton()
	{
		GRAV_ASSERT_EQ(s_Instance, nullptr); // Only one instance allowed at a time.
		s_Instance = static_cast<T*>(this);
	}
	template<typename T>
	inline singleton<T>::~singleton()
	{
		GRAV_ASSERT_NE(s_Instance, nullptr); // Instance of the singleton was not set.
		s_Instance = nullptr;
	}

	template<typename T>
	inline T& singleton<T>::getInstance()
	{
		GRAV_ASSERT_NE(s_Instance, nullptr); // Instance of the singleton was not set.
		return (*s_Instance);
	}
	template<typename T>
	inline T* singleton<T>::getInstancePtr()
	{
		GRAV_ASSERT_NE(s_Instance, nullptr); // Instance of the singleton was not set.
		return s_Instance;
	}
}

#if GRAVLib_COMPILER == GRAVLib_COMPILER_MSVC
#   pragma warning (pop)
#endif