#include "GAFrame.h"
#include <random>
#include <algorithm>
#include <iterator>

namespace GA
{
	namespace Select
	{
		void run(std::vector<SolutionWithScore> &pairs, GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting)
		{
			std::vector<bool> shade(select(pairs, compareFun, setting));

			unsigned int k(0);
			for (unsigned int i(0), j(shade.size()); i != j; ++i)
			{
				if (shade[i])
				{
					if (i != k)
					{
						pairs[k] = std::move(pairs[i]);
					}
					++k;
				}
			}
			pairs.erase(pairs.begin() + k, pairs.end());
		}

		std::vector<bool> select(const std::vector<SolutionWithScore> &pairs, GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting)
		{
			unsigned int targetNum(getCurrIterSolutionNum(pairs.size(), setting));
			std::vector<bool> ret(Operator::Tournament(pairs, targetNum, compareFun, setting));
			ret.front() = true;
			return ret;
		}

		unsigned int getCurrIterSolutionNum(unsigned int currSolutionNum, const Setting &setting)
		{
			return setting.currMaxSolutionNum / (1.0 +
				((long)setting.currMaxSolutionNum - (long)currSolutionNum) * exp(-rate * setting.bestContinueIter) / currSolutionNum);
		}

		namespace Operator
		{
			std::vector<bool> Tournament(const std::vector<SolutionWithScore> &pairs, unsigned int targetNum, GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting)
			{
				static std::random_device rd;
				static std::mt19937_64 gen(rd());

				unsigned int numOfEachGroup(targetNum / setting.solutionNumRange.first + 1);
				unsigned int numOfGroup(targetNum / numOfEachGroup + 1);

				std::poisson_distribution<> dis1(numOfEachGroup > 2 ? numOfEachGroup - 1 : numOfEachGroup);
				std::uniform_int_distribution<> dis2(0, numOfGroup - 1);
				std::vector<std::vector<unsigned int>> groups(numOfGroup, std::vector<unsigned int>());
				for (unsigned int i(0), j(pairs.size()); i != j; ++i)
					groups[dis2(gen)].push_back(i);

				std::sort(groups.begin(), groups.end(),
					[](const std::vector<unsigned int> &lop, const std::vector<unsigned int> &rop)
				{
					return lop.size() > rop.size();
				});
				for (auto lIt(groups.begin()), rIt(groups.end() - 1); lIt < rIt && rIt->empty(); ++lIt, --rIt)
				{
					std::random_shuffle(lIt->begin(), lIt->end());
					auto moveBgIt(lIt->begin()), moveEdIt(lIt->begin() + lIt->size() / 2);
					std::move(moveBgIt, moveEdIt, std::back_inserter(*rIt));
					lIt->erase(moveBgIt, moveEdIt);
				}

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

				return ret;
			}
		};
	};
};