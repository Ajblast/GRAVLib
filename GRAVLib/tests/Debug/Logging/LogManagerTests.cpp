// TODO: LogManger Tests

#include <gtest/gtest.h>

class LogManagerEnvironment : public ::testing::Environment {
public:
	~LogManagerEnvironment() override {}

	// Override this to define how to set up the environment.
	void SetUp() override {}

	// Override this to define how to tear down the environment.
	void TearDown() override {}
};

// Can set default logger
// Can set max verbosity
// Can set minimal flush verbosity

// Can get logger
// Can register logger
// Can remove logger