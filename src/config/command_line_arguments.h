#pragma once

#include <stdexcept>
#include <string>
#include <vector>

class CommandLineArgument {

public:
	CommandLineArgument() {
		this->name = "";
		this->value = "";
	}
	
	std::string name;
	std::string value;

};

class CommandLineArguments {

public:
	CommandLineArguments(int argc, char const** argv) {
		if(argc > 0) {
			this->programName = argv[0];
		}
		else {
			this->programName = "";
		}
		
		this->registerArguments(argc, argv);
	}
	
	std::string const& getProgramName() const {
		return this->programName;
	}
	
	std::vector<CommandLineArgument>::const_iterator begin() const {
		return this->arguments.cbegin();
	}
	
	std::vector<CommandLineArgument>::const_iterator end() const {
		return this->arguments.cend();
	}

private:
	void registerArguments(int argc, char const** argv) {
		for(int argumentIndex = 1; argumentIndex < argc; ++argumentIndex) {
			const char* currentArgv = argv[argumentIndex];
			if(currentArgv != nullptr) {
				this->currentArgv = currentArgv;
				this->registerArgument();
			}
		}
	}
	
	void registerArgument() {
		if(this->argumentNameIsValid()) {
			this->registerArgumentName();
		}
		else if(this->argumentValueIsPending()) {
			this->registerArgumentValue();
		}
	}
	
	bool argumentNameIsValid() const {
		return (this->currentArgumentName() != "");
	}
	
	void registerArgumentName() {
		std::string argumentName = this->currentArgumentName();
		this->arguments.push_back(CommandLineArgument());
		this->arguments.back().name = argumentName;
	}
	
	bool argumentValueIsPending() const {
		if(this->arguments.empty()) {
			return false;
		}
		else if(this->arguments.back().name.empty()){
			return false;
		}
		else {
			return (this->arguments.back().value.empty());
		}
	}
	
	void registerArgumentValue() {
		this->arguments.back().value = currentArgv;
	}
	
	std::string currentArgumentName() const {
		size_t currentArgumentLength = this->currentArgv.length();
		if(currentArgumentLength < 2) {
			return "";
		}
		else if(currentArgumentLength == 2) {
			return currentSingleCharacterArgumentName();
		}
		else {
			return currentMultiCharacterArgumentName();
		}
	}
	
	std::string currentSingleCharacterArgumentName() const {
		if((this->currentArgv.length() == 2) &&
		   (this->currentArgv[0] == '-') &&
		   (this->currentArgv[1] != '-')) {
			return std::string(1, this->currentArgv[1]);
		}
		else {
			return "";
		}
	}
	
	std::string currentMultiCharacterArgumentName() const {
		if((this->currentArgv.length() > 2) &&
		   (this->currentArgv[0] == '-') &&
		   (this->currentArgv[1] == '-')) {
			return this->currentArgv.substr(2);
		}
		else{
			return "";
		}
	}
	
	std::vector<CommandLineArgument> arguments;
	std::string currentArgv;
	std::string lastArgumentName;
	std::string programName;

};
