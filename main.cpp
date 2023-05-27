
#include <cmath>
#include <unordered_set>
#include <set>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

void print(std::unordered_set<int> const& set)
{
	auto itr = set.begin();
	std::cout << '[';

	if (itr != set.end())
	{
		std::cout << *itr;

		while (++itr != set.end())
		{
			std::cout << ',' << *itr ;
		}
	}
	std::cout << ']' << '\n';
}


void printStringsByInt(const std::unordered_map<int, std::unordered_set<std::string>>& myMap) {
	for (const auto& entry : myMap) {
		int key = entry.first;
		const std::unordered_set<std::string>& strings = entry.second;

		std::cout << key << "=" << std::endl;
		for (const std::string& str : strings) {
			std::cout << "\t" << str << std::endl;
		}
		std::cout << std::endl;
	}
}

bool containsOnlyDigits(int value, const std::unordered_set<int>& digitsSet) {
	while (value > 0) {
		int digit = value % 10;
		if (digitsSet.find(digit) == digitsSet.end()) {
			return false;
		}
		value /= 10;
	}
	return true;
}

std::string getShortestString(const std::unordered_set<std::string>& mySet, int target ) {
	std::string shortestString;
	bool firstString = true;

	for (const std::string& str : mySet) {
		if (firstString || str.length() < shortestString.length() && std::stoi(str)!= target) {
			shortestString = str;
			firstString = false;
		}
	}

	return shortestString;
}

std::unordered_map<int, std::unordered_set<std::string>> expressions;
bool get_path(std::unordered_set<int> set, int target)
{
	std::unordered_set<int> alphabet = set;
	for (const auto& element : set)
		expressions[element].insert(std::to_string(element));
	while (set.find(target) == set.end())
	{
		std::size_t initialSize = set.size();
		std::unordered_set<int > newset = set;
		int result;
		std::string temp;

		// concatination
		for (auto itr1 = set.begin(); itr1 != set.end(); ++itr1)
		{
			for (auto itr2 = set.begin(); itr2 != set.end(); ++itr2)
			{
				if (containsOnlyDigits(*itr2, alphabet))
					if (containsOnlyDigits(*itr1, alphabet)) {
						result = (*itr2 != 0)
							? *itr1 * pow(10, (int)log10(*itr2) + 1) + *itr2
							: *itr1 * 10;
						if (result <= target) {
							newset.insert(result);
							expressions[result].insert(std::to_string(result));
						}
					}
			}
		}
		// addition
		for (auto itr1 = set.begin(); itr1 != set.end(); ++itr1)
		{
			for (auto itr2 = itr1; itr2 != set.end(); ++itr2)
			{
				if (*itr2 == 0 || *itr1 == 0)
					continue;
				result = *itr1 + *itr2;
				if (result <= target)
				{
					temp = getShortestString(expressions[*itr1], target) + "+" + getShortestString(expressions[*itr2], target);
					newset.insert(result);
					expressions[result].insert(temp);
				}
			}
		}

		// multiplication
		for (auto itr1 = set.begin(); itr1 != set.end(); ++itr1)
		{
			for (auto itr2 = itr1; itr2 != set.end(); ++itr2)
			{
				if (*itr2 == 1 || *itr1 == 1)
					continue;
				std::string lefPart = getShortestString(expressions[*itr1], target);
				if (lefPart.find('+') != std::string::npos)
					continue;
				std::string righPart = getShortestString(expressions[*itr2], target);
				if (righPart.find('+') != std::string::npos)
					continue;
				result = *itr1 * *itr2;
				if (result <= target) {
					temp = lefPart + "*" + righPart;
					newset.insert(result);
					expressions[result].insert(temp);
				}
			}
		}

		if (initialSize == newset.size())
			return false;
		set = newset;
	}
		// print the final set of numbers
		//print(set);
		return true ;
}


int main() {
	expressions.clear();
	int  target = 10000;
	std::string answer = get_path(std::unordered_set<int>{1,0}, target) ? getShortestString(expressions[target], target) : "N";
	std::cout << answer;
	//if (argc < 3) {
	//	cout << "Usage: ./shortest_expression input_file output_file\n";
	//	return 1;
	//}

	//string inputFile = argv[1];
	//string outputFile = argv[2];
	std::string inputFile = "input.txt";
	std::string outputFile = "output.txt";
	std::ifstream input(inputFile);
	std::ofstream output(outputFile);

	if (!input.is_open()) {
		std::cout << "Failed to open input file.\n";
		return 1;
	}

	if (!output.is_open()) {
		std::cout << "Failed to open output file.\n";
		return 1;
	}

	int datasets;
	input >> datasets;

	for (int i = 0; i < datasets; i++) {
		int k, n,temp;
		input >> k;
		std::unordered_set<int> digits;

		for (int j = 0; j < k; j++) {
			input >> temp;
			digits.insert(temp);
		}
		input >> n;
		expressions.clear();
		std::string answer = get_path(digits , n) ? getShortestString(expressions[n], n) : "N";
		output << answer.length() << " " << answer << "\n";
	}

	input.close();
	output.close();
	return 0;
}
