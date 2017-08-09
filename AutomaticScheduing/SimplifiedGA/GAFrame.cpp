#include "GAFrame.h"
#include <functional>
#include <algorithm>

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
		}

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

				if (compareFun(output.bestPair, population->best))
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
	};
};