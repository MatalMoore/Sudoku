#pragma once

#include <string>

std::string lowercase(const std::string& arg) {
	std::string result = arg;
	for (int i = 0; i < result.size(); i++) {
		result[i] = tolower(result[i]);
	}
	return result;
}
