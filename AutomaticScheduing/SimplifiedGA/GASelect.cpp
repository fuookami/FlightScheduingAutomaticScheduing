#include "GAFrame.h"
#include <random>

namespace GA
{
	namespace Select
	{
		void run(std::vector<SolutionWithScore> &pairs, GenerateFlightPlan::SolutionCompareFunciont_t compareFun)
		{
			std::vector<bool> shade(select(pairs, compareFun));

			std::vector<SolutionWithScore> newPairs;
			for (unsigned int i(0), j(shade.size()); i != j; ++i)
			{
				if (shade[i])
					newPairs.push_back(std::move(pairs[i]));
			}
			pairs = newPairs;
		}

		std::vector<bool> select(const std::vector<SolutionWithScore> &pairs, GenerateFlightPlan::SolutionCompareFunciont_t compareFun)
		{
			unsigned int targetNum(getCurrIterSolutionNum(pairs.size()));
			std::vector<bool> ret(Operator::Tournament(pairs, targetNum, compareFun));
			return std::move(ret);
		}

		unsigned int getCurrIterSolutionNum(unsigned int currSolutionNum)
		{
			return currMaxSolutionNum / (1 + 
				(currMaxSolutionNum - currSolutionNum) / currSolutionNum * exp(-rate * bestContinueIter));
		}

		namespace Operator
		{
			std::vector<bool> Tournament(const std::vector<SolutionWithScore> &pairs, unsigned int targetNum, GenerateFlightPlan::SolutionCompareFunciont_t compareFun)
			{
				static std::random_device rd;
				static std::mt19937_64 gen(rd());

				unsigned int numOfEachGroup(targetNum / minSolutionNum + 1);
				unsigned int numOfGroup(targetNum / numOfEachGroup + 1);

				std::poisson_distribution<> dis1(numOfEachGroup > 2 ? numOfEachGroup - 1 : numOfEachGroup);
				std::uniform_int_distribution<> dis2(0, numOfGroup - 1);
				std::vector<std::vector<unsigned int>> groups(numOfGroup, std::vector<unsigned int>());
				for (unsigned int i(0), j(pairs.size()); i != j; ++i)
					groups[dis2(gen)].push_back(i);

				std::vector<bool> ret(pairs.size(), false);
				for (const std::vector<unsigned int> &group : groups)
				{
					std::set<unsigned int> thisGroup;
					if (group.size() == numOfEachGroup)
						thisGroup.insert(group.cbegin(), group.cend());
					else
					{
						for (unsigned int i(0), j(numOfEachGroup / 2); i != j; ++i)
							thisGroup.insert(group[i]);

						for (unsigned int i(0), j(dis1(gen)); i != j && thisGroup.size() != numOfEachGroup; ++i)
						{
							unsigned int thisSelect(dis1(gen));
							if (thisSelect >= group.size())
								continue;

							thisGroup.insert(group[thisSelect]);
						}
						for (unsigned int i(0), j(group.size()); i != j && thisGroup.size() != numOfEachGroup; ++i)
							thisGroup.insert(group[i]);
					}

					for (const unsigned int selected : thisGroup)
						ret[selected] = true;
				}

				return std::move(ret);
			}
		};
	};
};