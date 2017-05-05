#include "GeneticAlgorithmCrossOperation.h"

#include <set>
#include <array>
#include <algorithm>
#include <thread>
#include <random>

namespace UICodeGeneticAlgorithm
{
	namespace Cross
	{
		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair>& pairs, const SettingHelper & setting)
		{
			if (setting.crossMode == CrossMode::OneParent)
			{
				static auto setSoltPart([](UICodeSolt *solt, unsigned int pointA, unsigned int pointB,
					const UICodeSolt &parts)
				{
					for (unsigned int i(pointA), j(0), k(parts.size()); i != pointB && j != k; ++i, ++j)
						(*solt)[i] = parts[j];
				});

				static std::random_device rd;
				static std::mt19937_64 gen(rd());
				std::uniform_int_distribution<> dis(0, setting.length / 2 - 1);

				std::vector<std::array<unsigned int, 4>> points(pairs.size(), std::array<unsigned int, 4>());
				std::vector<UICodeSolt> newGeneration(pairs.size(), UICodeSolt());
				for (unsigned int i(0), j(pairs.size()); i != j; ++i)
				{
					newGeneration[i] = pairs[i].first;
					unsigned int length(dis(gen));
					unsigned int pointA(dis(gen) % (setting.length / 2 - length));
					unsigned int pointB(dis(gen) % (setting.length - 2 * length - pointA) + pointA + length);
					points[i] = { pointA, pointA + length, pointB, pointB + length };
				}

				std::vector<std::vector<UICodeSolt>> groups(pairs.size(), std::vector<UICodeSolt>(2, UICodeSolt()));
				std::vector<std::thread> threads;
				for (unsigned int i(0), j(pairs.size()); i != j; ++i)
				{
					groups[i][0] = UICodeSolt(newGeneration[i].cbegin() + points[i][0], newGeneration[i].cbegin() + points[i][1]);
					groups[i][1] = UICodeSolt(newGeneration[i].cbegin() + points[i][2], newGeneration[i].cbegin() + points[i][3]);
					threads.push_back(std::thread(cross, &groups[i], setting));
				}

				for (std::thread &thread : threads)
					thread.join();
				threads.clear();

				for (unsigned int i(0), j(newGeneration.size()); i != j; ++i)
				{
					threads.push_back(std::thread(setSoltPart, &newGeneration[i], points[i][0], points[i][1], groups[i][0]));
					threads.push_back(std::thread(setSoltPart, &newGeneration[i], points[i][2], points[i][3], groups[i][1]));
				}

				for (std::thread &thread : threads)
					thread.join();

				return std::move(newGeneration);
			}
			else
			{
				unsigned int parentNumber(Mode::getParentNumber(pairs, setting));
				unsigned int saveNum(pairs.size() % parentNumber);
				std::vector<UICodeSolt> newGeneration(saveNum, UICodeSolt());
				for (unsigned int i(0); i != saveNum; ++i)
					newGeneration[i] = pairs[i].first;

				std::vector<const UICodeSoltFitnessPair *> pPairs(pairs.size() - saveNum, nullptr);
				for (unsigned int i(saveNum), j(pairs.size()); i != j; ++i)
					pPairs[i - saveNum] = &pairs[i];
				std::random_shuffle(pPairs.begin(), pPairs.end());

				std::vector<std::vector<UICodeSolt>> groups((pPairs.size() - saveNum) / parentNumber,
					std::vector<UICodeSolt>(parentNumber, UICodeSolt()));
				std::vector<std::thread> threads;
				for (unsigned int i(0), j(groups.size()); i != j; ++i)
				{
					for (unsigned int k(0); k != parentNumber; ++k)
						groups[i][k] = pPairs[i * parentNumber + k]->first;
					threads.push_back(std::thread(cross, &groups[i], setting));
				}

				for (std::thread &thread : threads)
					thread.join();

				for (unsigned int i(0), j(groups.size()); i != j; ++i)
					for (unsigned int k(0); k != parentNumber; ++k)
						newGeneration.emplace_back(groups[i][k]);

				return std::move(newGeneration);
			}
		}

		void cross(std::vector<UICodeSolt> *solts, const SettingHelper &setting)
		{
			std::map<unsigned int, Operator::Fun *>::const_iterator currIt(Operator::funs.find(setting.crossOperator.toUInt()));
			if (currIt == Operator::funs.cend())
				Operator::defaultFun(*solts);
			else
				currIt->second(*solts);
		}

		namespace Mode
		{
			unsigned int getParentNumber(const std::vector<UICodeSoltFitnessPair>& pairs, const SettingHelper &setting)
			{
				static auto calAdapativeMultiParentNum([](const std::vector<UICodeSoltFitnessPair>& pairs,
					const SettingHelper &setting)->unsigned int
				{
					double x = 0.0;
					if (setting.fitnessType == FitnessType::BiggerBetter)
						x = (pairs.front().second - pairs.back().second) / (double)pairs.front().second;
					else
						x = (pairs.back().second - pairs.front().second) / (double)pairs.back().second;

					unsigned int parentNum(minMultiParentNum);
					parentNum += x / 0.1;
					if (parentNum > maxMultiParentNum)
						parentNum = maxMultiParentNum;
					return parentNum;
				});

				switch (setting.crossMode.toUInt())
				{
				case CrossMode::TwoParent:
					return 2;
				case CrossMode::MultiParent:
					return minMultiParentNum;
				case CrossMode::AdapativeMultiParent:
					return calAdapativeMultiParentNum(pairs, setting);
				default:
					return minMultiParentNum;
				}
			}
		};

		namespace Operator
		{
			namespace Funs
			{
				void OnePointFun(std::vector<UICodeSolt> &solt)
				{
					static std::random_device rd;
					static std::mt19937_64 gen(rd());
					std::uniform_int_distribution<> dis(0, solt.size() - 1);

					std::vector<bool> mark(solt.front().size(), false);
					for (unsigned int i(dis(gen)), j(mark.size()); i != j; ++i)
						mark[i] = true;

					CrossAreas(solt, mark);
				}

				void TwoPointFun(std::vector<UICodeSolt> &solt)
				{
					static std::random_device rd;
					static std::mt19937_64 gen(rd());
					std::uniform_int_distribution<> dis(0, solt.size() - 1);

					std::vector<bool> mark(solt.front().size(), false);
					unsigned int i(dis(gen)), j(dis(gen));
					if (i > j)
						std::swap(i, j);
					for (unsigned int k(i); k != j; ++k)
						mark[k] = true;

					CrossAreas(solt, mark);
				}

				void MultiPointFun(std::vector<UICodeSolt> &solt)
				{
					static std::random_device rd;
					static std::mt19937_64 gen(rd());
					std::poisson_distribution<> dis(1);
					std::uniform_int_distribution<> udis(0, solt.front().size() - 1);
					std::uniform_int_distribution<> bdis(0, 1);

					unsigned int pointNum(dis(gen) + 3);
					std::vector<bool> mark(solt.front().size(), false);
					std::set<unsigned int> pointSet;
					while (pointSet.size() != pointNum)
						pointSet.insert(udis(gen));
					std::vector<unsigned int> points(pointSet.cbegin(), pointSet.cend());
					std::sort(points.begin(), points.end());

					for (unsigned int i(0), j(points.size() - 1); i != j; ++i)
						if (bdis(gen) == 1)
							for (unsigned int k(points[i]), l(points[i + 1]); i != j; ++i)
								mark[k] = true;

					CrossAreas(solt, mark);
				}

				void UniformFun(std::vector<UICodeSolt> &solt)
				{
					static std::random_device rd;
					static std::mt19937_64 gen(rd());
					std::uniform_int_distribution<> bdis(0, 1);

					std::vector<bool> mark(solt.front().size(), false);
					for (unsigned int i(0), j(mark.size()); i != j; ++i)
						if (bdis(gen) == 1)
							mark[i] = true;

					CrossAreas(solt, mark);
				}

				void CycleFun(std::vector<UICodeSolt> &solt)
				{
					static std::random_device rd;
					static std::mt19937_64 gen(rd());
					std::uniform_int_distribution<> udis(0, solt.front().size() - 1);
					UICodeSolt firstCopy(solt.front());

					unsigned int point(udis(gen));
					for (unsigned int i(0), j(solt.size() - 1); i != j; ++i)
					{
						solt[i].erase(solt[i].begin(), solt[i].begin() + point);
						solt[i].insert(solt[i].end(), solt[i + 1].cbegin(), solt[i + 1].cbegin() + point);
					}
					solt.back().erase(solt.back().begin(), solt.back().begin() + point);
					solt.back().insert(solt.back().end(), firstCopy.cbegin(), firstCopy.cbegin() + point);
				}
			};

			void CrossAreas(std::vector<UICodeSolt> &solt, const std::vector<bool> &mark)
			{
				UICodeSolt firstCopy(solt.front());
				for (unsigned int i(0), j(solt.size() - 1); i != j; ++i)
				{
					for (unsigned int k(0), l(firstCopy.size()); k != l; ++k)
					{
						if (mark[k])
							solt[i][k] = solt[i + 1][k];
					}
				}
				for (unsigned int k(0), l(firstCopy.size()); k != l; ++k)
				{
					if (mark[k])
						solt.back()[k] = firstCopy[k];
				}
			}
		};
	};
};