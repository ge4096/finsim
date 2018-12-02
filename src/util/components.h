#pragma once

class Components {

public:
	Components() {
		this->configuration = nullptr;
		this->configKeyLookup = nullptr;
		this->configWriter = nullptr;
	}
	
	class Configuration* configuration;
	class ConfigKeyLookup* configKeyLookup;
	class ConfigWriter* configWriter;

};
