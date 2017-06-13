#include "GeneticAlgorithmSelectOperation.h"

#include <algorithm>
#include <numeric>
#include <set>
#include <random>
#include <thread>

namespace UICodeGeneticAlgorithm
{
	namespace Select
	{
		void run(std::vector<UICodeSoltFitnessPair>& pairs, CompareFun * compareFun, const SettingHelper & setting)
		{
			select(pairs, compareFun, getIndividualNum(pairs, setting), setting);
		}

		unsigned int getIndividualNum(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting)
		{
			std::map<unsigned int, Mode::Fun *>::const_iterator currIt(Mode::funs.find(setting.selectMode.toUInt()));
			unsigned int individualNum = 0;
			if (currIt == Mode::funs.cend())
				individualNum = Mode::defaultFun(pairs, setting);
			else
				individualNum = currIt->second(pairs, setting);
			if (individualNum < setting.individualNumber.first)
				individualNum *= (setting.individualNumber.first / individualNum + 1);
			return individualNum;
		}

		void select(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting)
		{
			if (individualNumber > pairs.size())
				Operator::copyPairsToSize(pairs, compareFun, individualNumber);

			std::map<unsigned int, Operator::Fun *>::const_iterator currIt(Operator::funs.find(setting.selectMode.toUInt()));
			if (currIt == Operator::funs.cend())
				Operator::defaultFun(pairs, compareFun, individualNumber, setting);
			else
				currIt->second(pairs, compareFun, individualNumber, setting);

			if (setting.individualNumber.first > pairs.size())
				Operator::copyPairsToSize(pairs, compareFun, individualNumber);
		}

		namespace Mode
		{
			namespace Funs
			{
				unsigned int Mode::Funs::StaticFun(const std::vector<UICodeSoltFitnessPair>& pairs, const SettingHelper & setting)
				{
					return setting.individualNumber.first;
				}

				unsigned int Mode::Funs::AdapativeDynamicFun(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting)
				{
					static auto FitnessArgument(
						[](const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting) -> double
					{
						static const double a(0.1);
						static const double b(0.6);
						static const double i(-(16.0 * a * (a + b) + 24.0 * a - 24.0 * b) / (b * pow(a + b, 3)));
						static const double j((double)a / b);
						double x = 0.0;
						if (setting.fitnessType == FitnessType::BiggerBetter)
							x = (pairs.front().second - pairs.back().second) / (double)pairs.front().second;
						else
							x = (pairs.back().second - pairs.front().second) / (double)pairs.back().second;
						if (x < ((b + 3 * a) / 4))
							return (3 * b + a) / (4 * b);
						else if (x > ((3 * b + a) / 4))
							return 1.0;
						else
							return i * pow(x, 3) + j * x + 0.5 + (b + 3.0 * a) / (8.0 * b);
					});

					static auto IndividualNumberArgument(
						[](const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting) -> double
					{
						unsigned int a(setting.individualNumber.first);
						unsigned int b(setting.individualNumber.second);
						if (pairs.size() < ((b + 3 * a) / 4))
							return 1.0;
						else if (pairs.size() > ((3 * b + a) / 4))
							return (b + 3.0 * a) / (4.0 * b);
						else
						{
							double i((16.0 * a * (a + b) + 24.0 * a - 24.0 * b) / (b * pow(a + b, 3)));
							double j(-(double)a / b);
							double x(pairs.size() + (a + b) / 4.0);
							return (i * pow(x, 3) + j * x + 0.5 + (b + 3.0 * a) / (8.0 * b)) / pairs.size() / 4;
						}
					});

					double fitnessArgument(FitnessArgument(pairs, setting));
					double individualNumberArgument(IndividualNumberArgument(pairs, setting));
					unsigned int orgRet(pairs.size() * fitnessArgument * individualNumberArgument);

					if (orgRet > setting.individualNumber.first)
						return orgRet;
					else if (orgRet < setting.individualNumber.second)
						return orgRet * (setting.individualNumber.first / orgRet + 1);
					else
						return orgRet / (orgRet / setting.individualNumber.second + 1);
				}
			};
		};

		namespace Operator
		{
			namespace Funs
			{
				void RouletteFun(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting)
				{
					static std::random_device rd;
					static std::mt19937_64 gen(rd());

					static auto randomGetPair([](UICodeSoltFitnessPair *pRet, const std::vector<unsigned int> fitnesses,
						const std::vector<UICodeSoltFitnessPair> &pairs, const unsigned int sum)
					{
						std::uniform_int_distribution<> dis(0, sum - 1);
						unsigned int thisValue(dis(gen));
						unsigned int i = 0;
						for (; thisValue > 0; ++i)
							thisValue -= fitnesses[i];
						--i;

						*pRet = pairs[i];
					});

					std::vector<unsigned int> fitnesses(pairs.size(), 0);
					for (unsigned int i(0), j(pairs.size()); i != j; ++i)
						fitnesses[i] = pairs[i].second;
					toDiscreasingOrder(fitnesses, setting);

					unsigned int sum(std::accumulate(fitnesses.cbegin(), fitnesses.cend(), 0));
					std::vector<UICodeSoltFitnessPair> newPairs(individualNumber, UICodeSoltFitnessPair());

					std::vector<std::thread> threads;
					for (unsigned int i(0); i != individualNumber; ++i)
						threads.push_back(std::thread(randomGetPair, &newPairs[i], fitnesses, pairs, sum));

					for (std::thread &thread : threads)
						thread.join();

					pairs = newPairs;
					std::sort(pairs.begin(), pairs.end(), compareFun);
				}

				void RankSelectFun(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting)
				{
					pairs.erase(pairs.begin() + individualNumber, pairs.end());
				}

				void TournamentFun(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting)
				{
					static auto groupTournament([](std::vector<UICodeSoltFitnessPair *> *group, CompareFun *compareFun)
					{
						std::sort(group->begin(), group->end(), [compareFun]
						(const UICodeSoltFitnessPair *pLop, const UICodeSoltFitnessPair *pRop) -> bool
						{
							return compareFun(*pLop, *pRop);
						});
						group->erase(group->begin() + EliteNum, group->end());
					});

					unsigned int groupNumber = individualNumber / EliteNum;
					unsigned int minIndividualNumber = GroupMinNum * groupNumber;

					if (pairs.size() < minIndividualNumber)
						copyPairsToSize(pairs, compareFun, minIndividualNumber);

					std::vector<std::vector<UICodeSoltFitnessPair *>> groups(groupNumber, std::vector<UICodeSoltFitnessPair *>());
					std::random_shuffle(pairs.begin(), pairs.end());
					for (unsigned int i(0), j(pairs.size()), m(0); i != j; ++i)
					{
						groups[m].push_back(&pairs[i]);
						m = (m + 1) % groupNumber;
					}

					std::vector<std::thread> threads;
					for (unsigned int i(0); i != groupNumber; ++i)
						threads.push_back(std::thread(groupTournament, &groups[i], compareFun));
					for (std::thread &thread : threads)
						thread.join();

					std::vector<UICodeSoltFitnessPair> newPairs;
					for (unsigned int i(0); i != groupNumber; ++i)
						for (unsigned int j(0); j != EliteNum; ++j)
							newPairs.emplace_back(*groups[i][j]);

					pairs = newPairs;
					std::sort(pairs.begin(), pairs.end(), compareFun);
				}
			};

			void copyPairsToSize(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, unsigned int individualNumber)
			{
				std::vector<UICodeSoltFitnessPair> pairsCopy(pairs);
				for (unsigned int i(0), j(individualNumber / pairs.size()); i != j; ++i)
					pairs.insert(pairs.end(), pairsCopy.cbegin(), pairsCopy.cend());
				for (UICodeSoltFitnessPair &pair : pairsCopy)
					pairs.emplace_back(pair);
				std::sort(pairs.begin(), pairs.end(), compareFun);
			}

			void toDiscreasingOrder(std::vector<unsigned int>& fitensses, const SettingHelper & setting)
			{
				if (setting.fitnessType == FitnessType::SmallerBetter)
				{
					unsigned int maximum(*std::max_element(fitensses.cbegin(), fitensses.cend()));

					for (unsigned int &fitness : fitensses)
						fitness = maximum - fitness;
				}
			}
		};
	};
};