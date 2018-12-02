#pragma once

#include "config/command_line_arguments.h"
#include "test/unit_test.h"

class CommandLineArgumentsTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(CommandLineArgumentsTest);
		RUN_TEST_CASE(noArguments);
		RUN_TEST_CASE(oneSingleCharacterArgument);
		RUN_TEST_CASE(oneMultiCharacterArgument);
		RUN_TEST_CASE(singleCharacterArgumentWithValue);
		RUN_TEST_CASE(multiCharacterArgumentWithValue);
		RUN_TEST_CASE(multipleArguments);
		RUN_TEST_CASE(malformedArguments);
		END_TEST(CommandLineArgumentsTest);
	}

private:
	void noArguments() {
		char const* programName = "program";
		char const* argv[] = {programName};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		CommandLineArguments arguments(argc, argv);
		ASSERT_TRUE(arguments.begin() == arguments.end());
	}
	
	void oneSingleCharacterArgument() {
		char const* programName = "program";
		char const* argumentName = "-a";
		char const* argv[] = {programName, argumentName};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		CommandLineArguments arguments(argc, argv);
		auto argument = arguments.begin();
		ASSERT_EQUAL(argument->name, "a");
		ASSERT_EQUAL(argument->value, "");
		++argument;
		ASSERT_TRUE(argument == arguments.end());
	}
	
	void oneMultiCharacterArgument() {
		char const* programName = "program";
		char const* argumentName = "--abc";
		char const* argv[] = {programName, argumentName};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		CommandLineArguments arguments(argc, argv);
		auto argument = arguments.begin();
		ASSERT_EQUAL(argument->name, "abc");
		ASSERT_EQUAL(argument->value, "");
		++argument;
		ASSERT_TRUE(argument == arguments.end());
	}
	
	void singleCharacterArgumentWithValue() {
		char const* programName = "program";
		char const* argumentName = "-a";
		char const* argumentValue = "b";
		char const* argv[] = {programName, argumentName, argumentValue};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		CommandLineArguments arguments(argc, argv);
		auto argument = arguments.begin();
		ASSERT_EQUAL(argument->name, "a");
		ASSERT_EQUAL(argument->value, "b");
		++argument;
		ASSERT_TRUE(argument == arguments.end());
	}
	
	void multiCharacterArgumentWithValue() {
		char const* programName = "program";
		char const* argumentName = "--abc";
		char const* argumentValue = "def";
		char const* argv[] = {programName, argumentName, argumentValue};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		CommandLineArguments arguments(argc, argv);
		auto argument = arguments.begin();
		ASSERT_EQUAL(argument->name, "abc");
		ASSERT_EQUAL(argument->value, "def");
		++argument;
		ASSERT_TRUE(argument == arguments.end());
	}
	
	void multipleArguments() {
		char const* programName = (char*)"program";
		char const* singleNoValue = (char*)"-a";
		char const* singleWithValue = (char*)"-b";
		char const* singleValue = (char*)"cde";
		char const* multiWithValue = (char*)"--fgh";
		char const* multiValue = (char*)"i";
		char const* multiNoValue = (char*)"--jkl";
		char const* argv[] = {programName, singleNoValue, singleWithValue, singleValue,
		                                   multiWithValue, multiValue, multiNoValue};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		CommandLineArguments arguments(argc, argv);
		auto argument = arguments.begin();
		ASSERT_EQUAL(argument->name, "a");
		ASSERT_EQUAL(argument->value, "");
		++argument;
		ASSERT_EQUAL(argument->name, "b");
		ASSERT_EQUAL(argument->value, "cde");
		++argument;
		ASSERT_EQUAL(argument->name, "fgh");
		ASSERT_EQUAL(argument->value, "i");
		++argument;
		ASSERT_EQUAL(argument->name, "jkl");
		ASSERT_EQUAL(argument->value, "");
		++argument;
		ASSERT_TRUE(argument == arguments.end());
	}
	
	void malformedArguments() {
		char const* programName = (char*)"program";
		char const* notFlagged = (char*)"abc";
		char const* onlyOneHyphen = (char*)"-def";
		char const* invalidSingle = (char*)"--";
		char const* argv[] = {programName, notFlagged, onlyOneHyphen, invalidSingle};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		CommandLineArguments arguments(argc, argv);
		ASSERT_TRUE(arguments.begin() == arguments.end());
	}

};
