#include "GeneticAlgorithmMutationOperation.h"

#include <thread>
#include <random>

namespace UICodeGeneticAlgorithm::Mutation
{
	std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair>& pairs, const SettingHelper & setting)
	{
		double thisMutationRate(Mode::getMutationRate(pairs, setting));
		std::vector<UICodeSolt *> thisGeneration(pairs.size(), nullptr);

		std::vector<std::thread> threads;
		for (unsigned int i(0), j(thisGeneration.size()); i != j; ++i)
			threads.push_back(std::thread(mutate, &thisGeneration[i], pairs[i], thisMutationRate, setting));
		for (std::thread &thread : threads)
			thread.join();

		std::vector<UICodeSolt> newGeneration;
		for (unsigned int i(0), j(thisGeneration.size()); i != j; ++i)
		{
			if (thisGeneration[i] != nullptr)
			{
				newGeneration.push_back(std::move(*thisGeneration[i]));
				delete thisGeneration[i];
			}
		}

		return std::move(newGeneration);
	}

	void mutate(UICodeSolt **solts, const UICodeSoltFitnessPair &pair, const double rate, const SettingHelper &setting)
	{
		std::map<unsigned int, Operator::Fun *>::const_iterator currIt(Operator::funs.find(setting.mutationOperator.toUInt()));
		if (currIt == Operator::funs.cend())
			Operator::defaultFun(solts, rate, setting.range, pair);
		else
			currIt->second(solts, rate, setting.range, pair);
	}

	namespace Mode
	{
		double getMutationRate(const std::vector<UICodeSoltFitnessPair>& pairs, const SettingHelper &setting)
		{
			if (setting.mutationRateMode == MutationRateMode::Static)
				return maxMutationRate;
			else
			{
				static auto CalAdapativeMutationRate(
					[](const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting)->double 
				{
					double x = 0.0;
					if (setting.fitnessType == FitnessType::BiggerBetter)
						x = (pairs.front().second - pairs.back().second) / (double)pairs.front().second;
					else
						x = (pairs.back().second - pairs.front().second) / (double)pairs.back().second;

					if (x < 1E-7)
						return maxMutationRate;
					else
					{
						x = 1.0 / x;
						double rate = x * minMutationRate;

						if (rate > maxMutationRate)
							return maxMutationRate;
						else
							return rate;
					}
				});
			}
		}
	}

	namespace Operator
	{
		namespace Funs
		{
			void UniformFun(UICodeSolt **solt, const double rate, const unsigned int range, const UICodeSoltFitnessPair &pair)
			{
				static std::random_device rd;
				static std::mt19937_64 gen(rd());
				static std::uniform_real_distribution<> dis(0, 1);

				std::vector<bool> mark(pair.first.size(), false);
				bool flag = false;
				for (unsigned int i(0), j(mark.size()); i != j; ++i)
				{
					if (dis(gen) < rate)
					{
						mark[i] = true;
						flag = true;
					}
				}

				if (flag)
					MutateArea(solt, mark, range, pair);
				else
					*solt = nullptr;
			}

			void NonUniformFun(UICodeSolt **solt, const double rate, const unsigned int range, const UICodeSoltFitnessPair &pair)
			{
				double rangeCopy(range * (pair.first.size() / 100 + 1));

				static std::random_device rd;
				static std::mt19937_64 gen(rd());
				static std::uniform_real_distribution<> dis(0, 1);

				std::vector<bool> mark(pair.first.size(), false);
				bool flag = false;
				for (unsigned int i(0), j(mark.size()); i != j; ++i)
				{
					if (dis(gen) < rangeCopy)
					{
						mark[i] = true;
						flag = true;
						rangeCopy /= 2;
					}
				}

				if (flag)
					MutateArea(solt, mark, range, pair);
				else
					*solt = nullptr;
			}

			void GaussianFun(UICodeSolt **solt, const double rate, const unsigned int range, const UICodeSoltFitnessPair &pair)
			{
				static std::random_device rd;
				static std::mt19937_64 gen(rd());
				static std::normal_distribution<> dis(5, 2);

				std::vector<bool> mark(pair.first.size(), false);
				bool flag = false;
				for (unsigned int i(0), j(mark.size()); i != j; ++i)
				{
					if ((dis(gen)) / 10 < range)
					{
						mark[i] = true;
						flag = true;
					}
				}

				if (flag)
					MutateArea(solt, mark, range, pair);
				else
					*solt = nullptr;
			}
		}

		void MutateArea(UICodeSolt **solt, const std::vector<bool> &mark, const unsigned int range, const UICodeSoltFitnessPair &pair)
		{
			static std::random_device rd;
			static std::mt19937_64 gen(rd());

			std::uniform_int_distribution<> dis(0, range - 1);
			*solt = new UICodeSolt(pair.first);
			for (unsigned int i(0), j(mark.size()); i != j; ++i)
				(**solt)[i] = dis(gen);
		}
	}
}