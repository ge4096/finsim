#include "util/components.h"
#include "config/configuration.h"

int main(int argc, char const** argv) {
	Components components;
	Configuration configuration(components, argc, argv);
	components.configuration = &configuration;
	return 0;
}
