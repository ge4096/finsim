#pragma once

#include "config/config_keys.h"
#include <exception>
#include <string>
#include <unordered_map>

class ConfigKeyLookup {

public:
	class KeyNotFoundError: public std::exception {
	
	public:
		KeyNotFoundError() {}
		
		~KeyNotFoundError() {}
		
		char const* what() const noexcept override {
			return "No matching config key found";
		}
	
	};
	
	ConfigKeyLookup(ConfigKey const* keys) {
		this->keys = keys;
		this->loadKeys();
	}
	
	ConfigKey const& getByFullName(std::string const& name) const {
		return *(this->getMatch(name, this->keysByFullName));
	}
	
	ConfigKey const& getByShortName(std::string const& name) const {
		return *(this->getMatch(name, this->keysByShortName));
	}
	
	ConfigKey const& getById(ConfigKeys::ConfigId id) const {
		ConfigKeys::HashableConfigId hashableId = id;
		return *(this->getMatch(hashableId, this->keysById));
	}

private:
	template<typename T> using KeyMap = std::unordered_map<T, ConfigKey const*>;
	
	void loadKeys() {
		ConfigKey const* currentKey = keys;
		while(currentKey->type != ConfigType::Null) {
			this->keysByFullName[currentKey->fullKey] = currentKey;
			ConfigKeys::HashableConfigId hashableId = currentKey->id;
			this->keysById[hashableId] = currentKey;
			if(currentKey->isCommandLineArgument) {
				this->keysByShortName[currentKey->commandLineKey] = currentKey;
			}
			++currentKey;
		}
	}
	
	template<typename T> ConfigKey const* getMatch(T const& identifier,
	                                               KeyMap<T> const& keyMap) const {
		auto key = keyMap.find(identifier);
		if(key == keyMap.end()) {
			throw KeyNotFoundError();
		}
		else {
			return key->second;
		}
	}
	
	KeyMap<std::string> keysByFullName;
	KeyMap<std::string> keysByShortName;
	KeyMap<ConfigKeys::HashableConfigId> keysById;
	ConfigKey const* keys;

};
