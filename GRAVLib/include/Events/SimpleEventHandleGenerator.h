#pragma once

#include "GRAVLibCore.h"
#include "IEventHandleGenerator.h"

namespace GRAVLib::Events
{
	class GRAVLibAPI simpleEventHandleGenerator : IEventHandleGenerator
	{
	public:
		virtual ~simpleEventHandleGenerator();

		virtual eventHandle generate() override;
	private:
		eventHandle m_CurrentHandle = 0;
	};
}