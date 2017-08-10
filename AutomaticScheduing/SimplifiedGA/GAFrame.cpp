#define _CRT_SECURE_NO_WARNINGS

#include "GAFrame.h"
#include <functional>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>

namespace GA
{
	GenerateFlightPlan::OutputDatas run(const std::vector<Solution> &initialSolution, bool FaultToTerant,
		const std::pair<unsigned int, unsigned int> &_solutionNumRange, const std::pair<unsigned int, unsigned int> &_range, 
		GenerateFlightPlan::SolutionScoreFunction_t toScoreFun,
		GenerateFlightPlan::SolutionCompareFunciont_t compareFun)
	{
		Setting setting(_solutionNumRange, _range);

		Mutation::currMutationRate = Mutation::startMutationRate;

		auto populations(Population::generateInitialPopulation(initialSolution,
			FaultToTerant, toScoreFun));

		GenerateFlightPlan::OutputDatas output;
		output.bestPair = populations.front()->best;
		output.minAndMaxScoresOfIters.push_back(std::make_pair(
			populations.front()->paris.front().second, populations.front()->paris.back().second));

		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t = localtime(&tt);
		std::ostringstream sout;
		sout << "log" << t->tm_year + 1900 << "-" << t->tm_mon + 1 << "-" << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << ".txt";
		std::ofstream fout(sout.str());

		while (setting.iter != maxIter && setting.bestContinueIter != maxBestContinueIter)
		{
 			Population::run(populations, toScoreFun, FaultToTerant, compareFun, setting);

			if (Sub::refreshOutputs(output, populations, compareFun, setting))
			{
				setting.bestContinueIter = 0;
				setting.currMaxSolutionNum = setting.solutionNumRange.first;
			}
			else if (setting.currMaxSolutionNum < setting.solutionNumRange.second)
			{
				setting.currMaxSolutionNum += (setting.solutionNumRange.second - setting.solutionNumRange.first) * Select::rate;
				setting.currMaxSolutionNum = (setting.currMaxSolutionNum > setting.solutionNumRange.second) ? setting.solutionNumRange.second : setting.currMaxSolutionNum;
			}

			++setting.bestContinueIter;
			++setting.iter;

			Sub::printCurrIter(fout, output, setting);
		}

		fout.close();

		return std::move(output);
	};

	namespace Sub
	{
		bool refreshOutputs(GenerateFlightPlan::OutputDatas &output, std::vector<std::shared_ptr<PopulationData>> &populations,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting)
		{
			std::vector<unsigned int> extremumScore;
			std::vector<unsigned int> populationQuantityOfIters;

			bool ret(false);
			output.mutationRateOfIters.push_back(Mutation::getCurrIterMutationRate(setting));
			for (auto population : populations)
			{
				populationQuantityOfIters.push_back(population->paris.size());
				extremumScore.push_back(population->paris.front().second);
				extremumScore.push_back(population->paris.back().second);

				if (compareFun(population->best, output.bestPair))
				{
					ret = true;
					output.bestPair = population->best;
				}
			}

			std::sort(extremumScore.begin(), extremumScore.end());
			std::sort(populationQuantityOfIters.begin(), populationQuantityOfIters.end());
			output.minAndMaxScoresOfIters.push_back(std::make_pair
			(extremumScore.front(), extremumScore.back()));
			output.minAndMaxPopulationQuantityOfIters.push_back(std::make_pair
			(populationQuantityOfIters.front(), populationQuantityOfIters.back()));

			return ret;
		}

		void printCurrIter(std::ofstream &fout, const GenerateFlightPlan::OutputDatas &output, const Setting &setting)
		{
			std::cout << "------------------------------" << std::endl;
			std::cout << "Iterator: " << setting.iter << std::endl;
			std::cout << "Best Solution Continuing Iterator: " << setting.bestContinueIter << std::endl;
			std::cout << "Score: " << output.minAndMaxScoresOfIters.back().first << " ~ " << output.minAndMaxScoresOfIters.back().second << std::endl;
			std::cout << "Quantity: " << output.minAndMaxPopulationQuantityOfIters.back().first << " ~ " << output.minAndMaxPopulationQuantityOfIters.back().second << std::endl;
			std::cout << "Mutation Rate: " << output.mutationRateOfIters.back() << std::endl;
			std::cout << "Best Score: " << output.bestPair.second << std::endl;
			std::cout << "------------------------------" << std::endl << std::endl;

			fout << "------------------------------" << std::endl;
			fout << "Iterator: " << setting.iter << std::endl;
			fout << "Best Solution Continuing Iterator: " << setting.bestContinueIter << std::endl;
			fout << "Score: " << output.minAndMaxScoresOfIters.back().first << " ~ " << output.minAndMaxScoresOfIters.back().second << std::endl;
			fout << "Quantity: " << output.minAndMaxPopulationQuantityOfIters.back().first << " ~ " << output.minAndMaxPopulationQuantityOfIters.back().second << std::endl;
			fout << "Mutation Rate: " << output.mutationRateOfIters.back() << std::endl;
			fout << "Best Score: " << output.bestPair.second << std::endl;
			fout << "------------------------------" << std::endl << std::endl;
		}
	};
};