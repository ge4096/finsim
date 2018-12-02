#pragma once

#include <test/unit_test.h>
#include <test/util/date_test.h>
#include <test/util/dollars_test.h>
#include <test/config/argument_loader_test.h>
#include <test/config/command_line_arguments_test.h>
#include <test/config/config_key_lookup_test.h>
#include <test/config/config_value_factory_test.h>
#include <iostream>
#include <vector>

class UnitTests {

public:
	UnitTests() {
		this->tests.push_back(new DateTest());
		this->tests.push_back(new DollarsTest());
		this->tests.push_back(new CommandLineArgumentsTest());
		this->tests.push_back(new ConfigKeyLookupTest());
		this->tests.push_back(new ConfigValueFactoryTest());
		this->tests.push_back(new ArgumentLoaderTest());
	}
	
	~UnitTests() {
		for(auto& test: this->tests) {
			delete test;
		}
	}
	
	int runAll() {
		int testsFailed = 0;
		bool currentTestResult;
		for(auto& test: this->tests) {
			currentTestResult = test->run();
			if(currentTestResult == false) {
				++testsFailed;
				break;
			}
		}
		
		if(testsFailed == 0) {
			std::cout << "All tests passed" << std::endl;
		}
		else {
			std::cout << "Some tests failed" << std::endl;
		}
		return testsFailed;
	}

private:
	std::vector<UnitTest*> tests;

};
