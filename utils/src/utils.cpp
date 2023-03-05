#include "utils.hpp"
#include <filesystem>


std::string Utils::joinPath(std::vector<std::string> dirs) {
    std::filesystem::path full_path("");
	
	for (std::string dir: dirs) {
		full_path = full_path / std::filesystem::path(dir);
	}
	
	return full_path.string();
}
