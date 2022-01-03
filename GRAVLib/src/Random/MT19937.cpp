#include "Random/MT19937.h"

GRAVLib::Random::mt19937::mt19937()
{
	m_Generator = std::mt19937(m_Device());
}
GRAVLib::Random::mt19937::~mt19937()
{
}

void GRAVLib::Random::mt19937::seed(resultType seed)
{
	m_Generator.seed(seed);
}
GRAVLib::Random::mt19937::resultType GRAVLib::Random::mt19937::generate()
{
	return m_Generator();
}
void GRAVLib::Random::mt19937::discard(size_t amount)
{
	m_Generator.discard(amount);
}


GRAVLib::Random::mt19937_64::mt19937_64()
{
	m_Generator = std::mt19937_64(m_Device());
}
GRAVLib::Random::mt19937_64::~mt19937_64()
{

}

void GRAVLib::Random::mt19937_64::seed(resultType seed)
{
	m_Generator.seed(seed);
}
GRAVLib::Random::mt19937_64::resultType GRAVLib::Random::mt19937_64::generate()
{
	return m_Generator();
}
void GRAVLib::Random::mt19937_64::discard(size_t amount)
{
	m_Generator.discard(amount);
}