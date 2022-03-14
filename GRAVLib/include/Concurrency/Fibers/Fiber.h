#pragma once

#include "GRAVLibCore.h"
#include "FiberTypes.h"
#include "FiberID.h"
#include <functional>
#include <format>

namespace GRAVLib::Concurrency::Fibers
{
	// TODO: Add the potential for there to be fiber's with different stack sizes

	// A fiber is a piece of execution context with its own stack that is called explicitly
	class GRAVLibAPI fiber
	{
	public:
		// Callback for fiber function
		typedef std::function<void(fiber*)> fiberCallback;
		//using fiberCallback = void(*)(fiber*);
	public:
		fiber();
		fiber(fiberCallback callback);
		fiber(fiberCallback callback, const std::string& name);
		fiber(const fiber& other) = delete;
		fiber(fiber&& other) noexcept;
		fiber& operator=(const fiber& other) = delete;
		fiber& operator=(fiber&& other) noexcept;
		~fiber();


		void spawn(fiberCallback callback);
		// Initialize the current thread and make it a fiber
		void initializeFromCurrentThread();
		// Convert the current fiber to a thread
		void convertToThread();

		// Switch to a fiber from this current one
		void switchTo(fiber& fiber);

		#pragma region Getters/Setters
		// Get the fiber's identifying information
		const fiberID getID() const;
		// Is the current fiber valid
		const bool valid() const;

		// Get the fiber's index
		const fiberIndex_t getFiberIndex() const;
		// Set the fiber's index
		void setFiberIndex(fiberIndex_t index);
		// Set the fiber's handle
		const fiberHandle_t getFiberHandle() const;

		// Get the fiber's name
		const std::string& name() const;
		#pragma endregion

	private:
		void close();

	private:
		fiberID m_ID;		// Fiber identifying information
		std::string m_Name;	// Fiber name

		bool m_IsThreadFiber = false;	// Is this a fiber created from a thread
	};

	inline const fiberID fiber::getID() const { return m_ID; }
	inline const bool fiber::valid() const { return getID().m_Handle; }
	inline const fiberIndex_t fiber::getFiberIndex() const { return getID().m_Index; }
	inline void fiber::setFiberIndex(fiberIndex_t index) { m_ID.m_Index = index; }
	inline const fiberHandle_t fiber::getFiberHandle() const { return getID().m_Handle; }
	inline const std::string& fiber::name() const { return m_Name; }
}

template<>
struct std::formatter<GRAVLib::Concurrency::Fibers::fiber> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Fibers::fiber& fiber, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[ID: {} | Name: {} | ThreadFiber: {}]", fiber.m_ID, fiber.m_Name, fiber.m_IsThreadFiber);
	}
};
