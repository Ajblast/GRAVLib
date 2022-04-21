#pragma once

#define GRAVLib_CONCAT_STRING_IMPL(s1, s2) s1##s2
#define GRAVLib_CONCAT_STRING(s1, s2) GRAVLib_CONCAT_STRING_IMPL(s1, s2)
#define GRAVLib_ANONYMOUS_VARIABLE(str) GRAVLib_CONCAT_STRING(str, __LINE__)

#define GRAVLib_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define GRAVLib_BIT(value) (1 << value)
