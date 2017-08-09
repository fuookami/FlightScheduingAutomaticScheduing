#include "GAFrame.h"
#include <thread>
#include <algorithm>

namespace GA
{
	namespace Population
	{
		std::vector<std::shared_ptr<PopulationData>> generateInitialPopulation(const std::vector<Solution> &initialSolution, bool FaultToTerant,
			GenerateFlightPlan::SolutionScoreFunction_t toScoreFun)
		{
			std::shared_ptr<PopulationData> pPopulation(new PopulationData());
			pPopulation->paris = toScoreFun(initialSolution, FaultToTerant);
			pPopulation->best = pPopulation->paris.front();

			std::vector<std::shared_ptr<PopulationData>> ret;
			ret.push_back(pPopulation);
			for (unsigned int i(1); i != PopulationNum; ++i)
				ret.push_back(std::shared_ptr<PopulationData>(new PopulationData(*pPopulation)));

			return std::move(ret);
		}

		void run(std::vector<std::shared_ptr<PopulationData>> &populations, GenerateFlightPlan::SolutionScoreFunction_t toScoreFun, bool FaultToTerant,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting)
		{
			std::vector<std::thread> threads;

			//for (unsigned int i(0); i != PopulationNum; ++i)
			//	iteration(populations[i].get(), toScoreFun, FaultToTerant, compareFun, setting);

			for (unsigned int i(0); i != PopulationNum; ++i)
				threads.push_back(std::thread(
					iteration, populations[i].get(), toScoreFun, FaultToTerant, compareFun, setting));
			
			for (unsigned int i(0); i != PopulationNum; ++i)
				threads[i].join();

			comunicate(populations, compareFun);

			return;
		}

		void iteration(PopulationData *pPopulation, GenerateFlightPlan::SolutionScoreFunction_t toScoreFun, bool FaultToTerant,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting)
		{
			PopulationData &population(*pPopulation);
			Select::run(population.paris, compareFun, setting);

			auto newIterAfterCross(Cross::run(population.paris, setting));
			auto newIterWithScoreAfterCross(toScoreFun(newIterAfterCross, FaultToTerant));
			population.paris.insert(population.paris.end(), newIterWithScoreAfterCross.begin(), newIterWithScoreAfterCross.end());

			auto newIterAfterMutation(Mutation::run(population.paris, setting));
			if (!newIterAfterMutation.empty())
			{
				auto newIterWithScoreAfterMutation(toScoreFun(newIterAfterMutation, FaultToTerant));
				population.paris.insert(population.paris.end(), newIterWithScoreAfterMutation.begin(), newIterWithScoreAfterMutation.end());
			}

			std::sort(population.paris.begin(), population.paris.end(),
				[compareFun](const SolutionWithScore &lps, const SolutionWithScore &rps)->bool
			{
				return compareFun(lps, rps);
			});

			if (compareFun(population.paris.front(), population.best))
				population.best = population.paris.front();
		}

		void comunicate(std::vector<std::shared_ptr<PopulationData>> &populations, GenerateFlightPlan::SolutionCompareFunciont_t compareFun)
		{
			for (unsigned int i(0); i != PopulationNum; ++i)
			{
				for (unsigned int j(0); j != PopulationNum; ++j)
				{
					if (i != j)
					{
						if (compareFun(populations[j]->best, populations[i]->best))
						{
							populations[i]->best = populations[j]->best;
							populations[i]->paris.insert(populations[i]->paris.begin(), populations[j]->best);
						}
						else
						{
							const SolutionWithScore &thisBest(populations[j]->best);
							auto insertIt(std::find_if(populations[i]->paris.begin(), populations[i]->paris.end(),
								[compareFun, &thisBest](const SolutionWithScore &lps)->bool
							{
								return compareFun(lps, thisBest);
							}));
							populations[i]->paris.insert(insertIt, thisBest);
						}
					}
				}
			}
		}
	};
};