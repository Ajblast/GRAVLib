#include "Concurrency/Threads/TLS.h"

GRAVLib::Concurrency::Threads::tls::tls()
{
}
GRAVLib::Concurrency::Threads::tls::tls(tls&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Concurrency::Threads::tls& GRAVLib::Concurrency::Threads::tls::operator=(tls&& other) noexcept
{
	if (this != &other)
	{
		m_Fiber = std::move(other.m_Fiber);
		m_CurrentFiber = std::move(other.m_CurrentFiber);

		m_PreviousFiberID = std::move(other.m_PreviousFiberID);
		m_PreviousFiberStored = std::move(other.m_PreviousFiberStored);
		m_PreviousFiberDestination = std::move(other.m_PreviousFiberDestination);
	}

	return *this;
}
GRAVLib::Concurrency::Threads::tls::~tls()
{
}