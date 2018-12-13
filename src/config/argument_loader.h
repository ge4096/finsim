#pragma once

#include "config/command_line_arguments.h"
#include "config/config_key_lookup.h"
#include "config/config_writer.h"
#include "util/components.h"

class ArgumentLoader {

public:
	ArgumentLoader(Components& components, int argc, char const** argv) {
		this->keyLookup = components.configKeyLookup;
		this->configWriter = components.configWriter;
		this->arguments = new CommandLineArguments(argc, argv);
		this->setConfigFilename = false;
		this->loadArguments();
	}
	
	~ArgumentLoader() {
		delete this->arguments;
	}

private:
	void loadArguments() {
		for(auto const& argument: *(this->arguments)) {
			this->loadArgument(argument);
		}
		if(!this->setConfigFilename) {
			this->setDefaultConfigFilename();
		}
	}
	
	void loadArgument(CommandLineArgument const& argument) {
		std::string const& name = argument.name;
		ConfigKey const& matchingKey = this->keyLookup->getByShortName(name);
		if(argument.value.empty()) {
			this->configWriter->writeValue(matchingKey, "true");
		}
		else {
			this->configWriter->writeValue(matchingKey, argument.value);
		}
		
		if(matchingKey.id == ConfigKeys::ConfigFilename) {
			this->setConfigFilename = true;
		}
	}
	
	void setDefaultConfigFilename() {
		ConfigKey const& configFilenameKey =
			this->keyLookup->getById(ConfigKeys::ConfigFilename);
		std::string defaultConfigFilename = this->arguments->getProgramName();
		defaultConfigFilename += ".cfg";
		this->configWriter->writeValue(configFilenameKey,
		                               defaultConfigFilename);
		this->setConfigFilename = true;
	}
	
	CommandLineArguments* arguments;
	ConfigKeyLookup* keyLookup;
	ConfigWriter* configWriter;
	bool setConfigFilename;

};
