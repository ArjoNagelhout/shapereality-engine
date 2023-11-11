#include <iostream>
#include <fstream>
#include <filesystem>

#include "yaml-cpp/yaml.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

	std::filesystem::path const directoryPath = "/Users/arjonagelhout/Documents/Experiments/bored_engine/";
	std::string fileName = "config.yaml";

	std::filesystem::path path = std::filesystem::path(directoryPath).append(fileName);

	std::ofstream outStream = std::ofstream(path);

	YAML::Node config;

	config["username"] = "Arjo";
	config["password"][0] = "Testing";
	config["something"][0][0] = "Dingetjes";
	config["something"][0][1] = "auaua";
	config["something"][1][0] = "Somethign";

	outStream << config;

	return 0;
}
