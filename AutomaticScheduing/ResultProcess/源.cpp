#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using uint32 = unsigned int;

std::vector<std::pair<uint32, uint32>> quantities;
std::vector<std::pair<uint32, uint32>> scores;
std::vector<uint32> bestContinuingIterators;
std::vector<double> mutationRates;
uint32 globalBestScore(0xffffffff);

void input(void);
void output(void);

int main(void)
{
	input();
	output();

	system("pause");
	return 0;
}

void input(void)
{
	uint32 bestContinuingIterator(1);

	std::ifstream fin("result.txt");
	uint32 iterators(0);
	fin >> iterators;

	for (uint32 i(0); i != iterators; ++i, ++bestContinuingIterator)
	{
		uint32 minQuatity(0), maxQuatity(0), bestScore(0), wrostScore(0);
		double mutationRate(0);

		fin >> minQuatity >> maxQuatity >> bestScore >> wrostScore >> mutationRate;

		if (bestScore < globalBestScore)
		{
			globalBestScore = bestScore;
			bestContinuingIterator = 1;
		}

		quantities.push_back(std::make_pair(minQuatity, maxQuatity));
		scores.push_back(std::make_pair(bestScore, wrostScore));
		mutationRates.push_back(mutationRate);
		bestContinuingIterators.push_back(bestContinuingIterator);
	}

	fin.close();
}

void output(void)
{
	std::ofstream fout("result\\ResultProcess.json");
	fout << "{" << std::endl;

	fout << "\t\"Quantities\": [" << std::endl;
	for (uint32 i(0), j(quantities.size()); i != j; ++i)
	{
		fout << "\t\t[" << quantities[i].first << ", " << quantities[i].second << "]";
		if (i + 1 != j)
			fout << ",";
		fout << std::endl;
	}
	fout << "\t]," << std::endl;

	fout << "\t\"Scores\": [" << std::endl;
	for (uint32 i(0), j(scores.size()); i != j; ++i)
	{
		fout << "\t\t[" << scores[i].first << ", " << scores[i].second << "]";
		if (i + 1 != j)
			fout << ",";
		fout << std::endl;
	}
	fout << "\t]," << std::endl;

	fout << "\t\"BestContinuingIterators\": [" << std::endl;
	for (uint32 i(0), j(bestContinuingIterators.size()); i != j; )
	{
		uint32 thisMax = j - i;
		thisMax = thisMax > 5 ? 5 : thisMax;
		fout << "\t\t";
		for (uint32 m(i), n(i + thisMax); m != n; ++m)
		{
			fout << bestContinuingIterators[m];
			if (m + 1 != j)
			{
				fout << ", ";
			}
		}
		fout << std::endl;

		i += thisMax;
	}
	fout << "\t]," << std::endl;

	fout << "\t\"MutationRate\": [" << std::endl;
	for (uint32 i(0), j(mutationRates.size()); i != j; )
	{
		uint32 thisMax = j - i;
		thisMax = thisMax > 5 ? 5 : thisMax;
		fout << "\t\t";
		for (uint32 m(i), n(i + thisMax); m != n; ++m)
		{
			fout << mutationRates[m] * 100;
			if (m + 1 != j)
			{
				fout << ", ";
			}
		}
		fout << std::endl;

		i += thisMax;
	}
	fout << "\t]";

	fout << "}" << std::endl;
	fout.close();
}