#include "GAFrame.h"
#include <functional>
#include <algorithm>

namespace GA
{
	GenerateFlightPlan::OutputDatas run(const std::vector<Solution> &initialSolution, bool FaultToTerant, 
		std::pair<unsigned int, unsigned int> range, GenerateFlightPlan::SolutionScoreFunction_t toScoreFun, 
		GenerateFlightPlan::SolutionCompareFunciont_t compareFun)
	{
		Sub::init(initialSolution, range);
		std::vector<Population::Population> populations(Population::generateInitialPopulation(initialSolution, 
			FaultToTerant, toScoreFun));
		
		GenerateFlightPlan::OutputDatas output;
		output.bestPair = populations.front().best;

		while (iter != maxIter && bestContinueIter != maxBestContinueIter)
		{
			Population::run(populations, toScoreFun, FaultToTerant, compareFun);
			
			if (Sub::refreshOutputs(output, populations, compareFun))
			{
				bestContinueIter = 0;
				currMaxSolutionNum = minSolutionNum;
			}
			else if (currMaxSolutionNum < maxSolutionNum)
			{
				currMaxSolutionNum += (maxSolutionNum - minSolutionNum) * Select::rate;
				currMaxSolutionNum = (currMaxSolutionNum > maxSolutionNum) ? maxSolutionNum : currMaxSolutionNum;
			}

			++bestContinueIter;
			++iter;
		}

		return std::move(output);
	};

	namespace Sub
	{
		void init(const std::vector<Solution> &initialSolution, std::pair<unsigned int, unsigned int> _range)
		{
			iter = 0;
			bestContinueIter = 0;

			minSolutionNum = initialSolution.size() * 3 / 4;
			currMaxSolutionNum = minSolutionNum;
			maxSolutionNum = initialSolution.size() * 2;

			range = _range;
		}

		bool refreshOutputs(GenerateFlightPlan::OutputDatas &output, std::vector<GA::Population::Population> &populations,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun)
		{
			std::vector<unsigned int> extremumScore;
			std::vector<unsigned int> populationQuantityOfIters;
			
			bool ret(false);
			output.mutationRateOfIters.push_back(Mutation::getCurrIterMutationRate());
			for (auto population : populations)
			{
				populationQuantityOfIters.push_back(population.paris.size());
				extremumScore.push_back(population.paris.front().second);
				extremumScore.push_back(population.paris.back().second);

				if (compareFun(population.best, output.bestPair))
				{
					ret = true;
					output.bestPair = population.best;
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
	};
};