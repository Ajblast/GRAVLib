#pragma once

#include "GRAVLibCore.h"
#include "EventHandle.h"

namespace GRAVLib::Events
{
	class GRAVLibAPI IEventHandleGenerator
	{
	public:
		virtual ~IEventHandleGenerator() {}

		virtual eventHandle generate() = 0;
	};
}