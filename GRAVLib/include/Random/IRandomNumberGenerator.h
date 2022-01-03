#pragma once

#include "GRAVLibCore.h"

namespace GRAVLib::Random
{
	template<typename T>
	class GRAVLibAPI IRandomNumberGenerator
	{
	public:
		using resultType = T;
	public:
		virtual ~IRandomNumberGenerator() {}

		virtual void seed(resultType seed) = 0;

		virtual resultType generate() = 0;
		virtual void discard(size_t amount) = 0;
	};
}