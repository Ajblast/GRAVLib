#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Events/EventHandle.h"

namespace GRAVLib::Events
{
	class GRAVLibAPI IEventHandleGenerator
	{
	public:
		virtual ~IEventHandleGenerator() {}

		virtual eventHandle generate() = 0;
	};
}