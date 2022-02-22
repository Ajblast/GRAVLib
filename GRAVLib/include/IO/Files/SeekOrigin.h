#pragma once
#include <iostream>

namespace GRAVLib::IO
{
	enum class seekOrigin
	{
		beg = std::ios_base::beg,
		cur = std::ios_base::cur,
		end = std::ios_base::end
	};
}