#pragma once
#include <string>
#include <vector>

namespace CSVHelper
{
	inline std::vector<int> Split(const std::string& str, char delim = ',')
	{
		std::vector<int> retVal;
		size_t i = 0;
		const size_t n = str.size();

		while (i < n)
		{
			if (str[i] == delim || str[i] == '\n' || str[i] == '\r' || std::isspace((unsigned char)str[i])) {
				++i;
				continue;
			}

			size_t j = i;

			if (j < n && (str[j] == '+' || str[j] == '-')) ++j;

			while (j < n && std::isdigit((unsigned char)str[j])) ++j;


			if (j > i) {
				retVal.push_back(std::stoi(str.substr(i, j - i)));
				i = j;
			} else {

				++i;
			}
		}

		return retVal;
	}
} // namespace CSVHelper
