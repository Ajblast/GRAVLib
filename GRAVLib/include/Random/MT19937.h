#pragma once

#include "GRAVLibCore.h"
#include "IRandomNumberGenerator.h"
#include <random>

namespace GRAVLib::Random
{
	class GRAVLibAPI mt19937 : public IRandomNumberGenerator<uint_fast32>
	{
	public:
		mt19937();
		virtual ~mt19937();

		virtual void seed(resultType seed) override;

		virtual resultType generate() override;
		virtual void discard(size_t amount) override;

	private:
		std::random_device m_Device;
		std::mt19937 m_Generator;
	};

	class GRAVLibAPI mt19937_64 : public IRandomNumberGenerator<uint_fast64>
	{
	public:
		mt19937_64();
		virtual ~mt19937_64();

		virtual void seed(resultType seed) override;

		virtual resultType generate() override;
		virtual void discard(size_t amount) override;

	private:
		std::random_device m_Device;
		std::mt19937_64 m_Generator;
	};

}