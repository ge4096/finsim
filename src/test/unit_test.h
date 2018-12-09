#pragma once

#include <iostream>

#define TEST_PRINT_INDENT "       "

#define START_TEST(test) {                             \
    std::cout << TEST_PRINT_INDENT #test << std::endl; \
    std::cout << "================" << std::endl;      \
}

#define RUN_TEST_CASE(test) {                      \
    this->lastTestCasePassed = true;               \
    this->setUp();                                 \
    this->test();                                  \
    this->tearDown();                              \
    if(this->lastTestCasePassed) {                 \
        std::cout << "  PASS " #test << std::endl; \
    }                                              \
    else {                                         \
        std::cout << std::endl;                    \
        std::cout << "! FAIL " #test << std::endl; \
        return false;                              \
    }                                              \
}

#define END_TEST(test) {                          \
    std::cout << "================" << std::endl; \
    std::cout << "  PASS " #test << std::endl;    \
    std::cout << std:: endl << std::endl;         \
    return true;                                  \
}

#define ASSERTION_FAILURE_HEADER() {                                 \
    std::cout << std::endl << TEST_PRINT_INDENT;                     \
    std::cout << "At " __FILE__ ":" << __LINE__ << ":" << std::endl; \
}

#define ASSERT_TRUE(condition) {                                             \
    if(!(condition)) {                                                       \
        ASSERTION_FAILURE_HEADER();                                          \
        std::cout << TEST_PRINT_INDENT #condition " was false" << std::endl; \
        this->lastTestCasePassed = false;                                    \
    }                                                                        \
}

#define ASSERT_FALSE(condition) {                                           \
    if(condition) {                                                         \
        ASSERTION_FAILURE_HEADER();                                         \
        std::cout << TEST_PRINT_INDENT #condition " was true" << std::endl; \
        this->lastTestCasePassed = false;                                   \
    }                                                                       \
}

#define ASSERT_EQUAL(actual, expected) {                                                  \
    if((actual) != (expected)) {                                                          \
        ASSERTION_FAILURE_HEADER();                                                       \
        std::cout << TEST_PRINT_INDENT #actual " does not match " #expected << std::endl; \
        std::cout << TEST_PRINT_INDENT "Actual: " << (actual) << std::endl;               \
        std::cout << TEST_PRINT_INDENT "Expected: " << (expected) << std::endl;           \
        this->lastTestCasePassed = false;                                                 \
    }                                                                                     \
}

#define ASSERT_THROW(command, error) {                                        \
    try {                                                                     \
        command;                                                              \
        ASSERTION_FAILURE_HEADER();                                           \
        std::cout << TEST_PRINT_INDENT #error " was not thrown" << std::endl; \
        this->lastTestCasePassed = false;                                     \
    }                                                                         \
    catch(error const&) {}                                                    \
}

class UnitTest {

public:
	virtual ~UnitTest() {}
	
	virtual bool run() = 0;
	
	virtual void setUp() {}
	
	virtual void tearDown() {}

protected:
	bool lastTestCasePassed;

};
