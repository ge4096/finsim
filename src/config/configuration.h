#pragma once

#include "config/argument_loader.h"
#include "config/config_keys.h"
#include "config/config_key_lookup.h"
#include "config/config_value_factory.h"
#include "config/config_writer.h"
#include "util/components.h"
#include <string>
#include <unordered_map>

class Configuration: public ConfigWriter {

public:
	Configuration(Components& components, int argc, char const** argv) {
		components.configWriter = this;
		
		this->keyLookup = new ConfigKeyLookup(ConfigKeys::keys);
		components.configKeyLookup = this->keyLookup;
		
		this->valueFactory = new ConfigValueFactory();
		this->argumentLoader = new ArgumentLoader(components, argc, argv);
	}
	
	~Configuration() {
		delete this->argumentLoader;
		delete this->valueFactory;
		delete this->keyLookup;
	}
	
	template<typename T> T const& getValue(ConfigKeys::ConfigId id) const {
		throw std::runtime_error("Unrecognized configuration type");
	}
	
	void /* ConfigWriter:: */ writeValue(ConfigKey const& key,
	                                     std::string const& value) override {
		auto matchingItem = this->configurationData.find(key.id);
		if(matchingItem == this->configurationData.end()) {
			void const* parsedValue = this->valueFactory->parseValue(value,
			                                                         key.type);
			this->configurationData[key.id] = parsedValue;
		}
	}

private:
	template<typename T> T const& checkTypeAndGetValue(ConfigKeys::ConfigId id,
	                                                   ConfigType type) const {
		ConfigKey const& fullKey = this->keyLookup->getById(id);
		if(fullKey.type == type) {
			return *(static_cast<T const*>(this->configurationData.at(id)));
		}
		else {
			throw std::runtime_error("Configuration value type mismatch");
		}
	}
	
	ConfigKeyLookup* keyLookup;
	ArgumentLoader* argumentLoader;
	ConfigValueFactory* valueFactory;
	std::unordered_map<ConfigKeys::ConfigId, void const*> configurationData;

};

int const& Configuration::getValue(ConfigKeys::ConfigId id) const {
	ConfigType constexpr type = ConfigType::Integer;
	return this->checkTypeAndGetValue<int>(id, type);
}

unsigned int const& Configuration::getValue(ConfigKeys::ConfigId id) const {
	ConfigType constexpr type = ConfigType::UnsignedInteger;
	return this->checkTypeAndGetValue<unsigned int>(id, type);
}

double const& Configuration::getValue(ConfigKeys::ConfigId id) const {
	ConfigType constexpr type = ConfigType::FloatingPoint;
	return this->checkTypeAndGetValue<double>(id, type);
}

bool const& Configuration::getValue(ConfigKeys::ConfigId id) const {
	ConfigType constexpr type = ConfigType::Boolean;
	return this->checkTypeAndGetValue<bool>(id, type);
}

std::string const& Configuration::getValue(ConfigKeys::ConfigId id) const {
	ConfigType constexpr type = ConfigType::String;
	return this->checkTypeAndGetValue<std::string>(id, type);
}
