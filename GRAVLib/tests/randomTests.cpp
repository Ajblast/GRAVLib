#include <gtest/gtest.h>
#include "GRAVLibCore.h"
#include "Random/IRandomNumberGenerator.h"

namespace GRAVLib::Random
{
	//class simpleRandom : public IRandomNumberGenerator<int>
	//{
	//public:
	//	// Inherited via IRandomNumberGenerator
	//	virtual void seed(resultType seed) override
	//	{
	//	}
	//	virtual resultType generate() override
	//	{
	//		return resultType();
	//	}
	//	virtual void discard(size_t amount) override
	//	{
	//	}


	//};

	//TEST(IRandomNumberGenerator, CanSetSeed)
	//{
	//	simpleRandom sut = simpleRandom();

	//	int generatedValue = sut.generate();
	//}
	//TEST(IRandomNumberGenerator, CanGenerate)
	//{
	//	simpleRandom sut = simpleRandom();

	//	int generatedValue = sut.generate();
	//}
}