#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <cmath>

std::vector<std::pair<unsigned int, unsigned int>> pairs;
std::vector<unsigned int> counter;
unsigned int totalTimes(0);
std::string input("ResultTestInitialSolution.txt");
std::string sortOutput("Result\\SortedResultTestInitialSolution.txt");
std::string output("Result\\ProcessResultTestInitialSolution.json");

void load(void)
{
	std::ifstream fin(input);

	std::string line;
	while (getline(fin, line))
	{
		unsigned int result, times;
		std::istringstream sin(line);
		sin >> result >> times;
		pairs.push_back(std::make_pair(result, times));
	}
	fin.close();

	std::sort(pairs.begin(), pairs.end(), 
		[](const std::pair<unsigned int, unsigned int> &lps,
			const std::pair<unsigned int, unsigned int> &rps)->bool 
	{
		return lps.first < rps.first;
	});

	unsigned int maxTimes(0);
	for (unsigned int i(0), j(pairs.size()); i != j; ++i)
	{
		if (pairs[i].second > maxTimes)
			maxTimes = pairs[i].second;
		totalTimes += pairs[i].second;
		counter.push_back(totalTimes);
	}

	std::ofstream fout(sortOutput);
	for (unsigned int i(0), j(pairs.size()); i != j; ++i)
	{
		fout << std::setw(log(pairs.back().first) / log(10) + 1) << pairs[i].first << '\t' 
			<< pairs[i].second << '\t' 
			<< std::setprecision(4) << std::setw(6) << 100.0 / ((double)maxTimes / pairs[i].second) << "%\t"
			<< std::setw(log(totalTimes) / log(10) + 1) << counter[i] << '\t' 
			<< std::setprecision(5) << 100.0 / ((double)totalTimes / counter[i]) << '%' << std::endl;
	}
	fout.close();
}

void process(void);

int main(void)
{
	load();
	process();

	system("pause");
	return 0;
}

void process(void)
{
	std::ofstream fout(output);
	fout << "{" << std::endl;

	bool notFirstFlag(false);
	
	// output xAxis
	notFirstFlag = false;
	fout << "	\"xAsis\": [";
	for (const std::pair<unsigned int, unsigned int> &pair : pairs)
	{
		if (notFirstFlag)
			fout << ", ";

		fout << '\"' << pair.first << '\"';

		notFirstFlag = true;
	}
	fout << "]," << std::endl;

	// output value
	notFirstFlag = false;
	fout << "	\"Value\": [";
	for (const std::pair<unsigned int, unsigned int> &pair : pairs)
	{
		if (notFirstFlag)
			fout << ", ";

		fout << pair.second;

		notFirstFlag = true;
	}
	fout << "]," << std::endl;

	// output Percentage
	notFirstFlag = false;
	fout << "	\"Percentage\": [";
	for (const std::pair<unsigned int, unsigned int> &pair : pairs)
	{
		if (notFirstFlag)
			fout << ", ";

		fout << 100.0 * pair.second / 100000.0;

		notFirstFlag = true;
	}
	fout << "]," << std::endl;

	// output accumulate value
	notFirstFlag = false;
	fout << "	\"Accumulate_Value\": [";
	for (const unsigned int &count : counter)
	{
		if (notFirstFlag)
			fout << ", ";

		fout << count;

		notFirstFlag = true;
	}
	fout << "]," << std::endl;

	// output accumulate percentage
	notFirstFlag = false;
	fout << "	\"Accumulate_Percentage\": [";
	for (const unsigned int &count : counter)
	{
		if (notFirstFlag)
			fout << ", ";

		fout << 100.0 * count / 100000.0;

		notFirstFlag = true;
	}
	fout << "]" << std::endl;

	fout << "}" << std::endl;
	fout.close();
}