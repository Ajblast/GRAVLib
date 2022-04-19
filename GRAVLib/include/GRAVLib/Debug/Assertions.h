#pragma once

#ifdef GRAVLib_ASSERTIONS
	#define debugBreak() __debugbreak()

	#define GRAV_ASSERT_TRUE(expr) \
		if (expr) {} \
		else { \
			debugBreak(); \
		}
	#define GRAV_ASSERT_FALSE(expr) GRAV_ASSERT_TRUE(!(expr))
	#define GRAV_ASSERT_EQ(value1, value2) GRAV_ASSERT_TRUE(value1 == value2)
	#define GRAV_ASSERT_NE(value1, value2) GRAV_ASSERT_FALSE(value1 == value2)
#else
	#define GRAV_ASSERT_TRUE(expr)
	#define GRAV_ASSERT_FALSE(expr)
	#define GRAV_ASSERT_EQ(expr)
	#define GRAV_ASSERT_NE(expr)
#endif