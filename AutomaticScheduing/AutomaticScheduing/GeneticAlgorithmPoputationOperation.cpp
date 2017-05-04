#include "GeneticAlgorithmPoputationOperation.h"

#include <thread>
#include <algorithm>
#include <random>

namespace UICodeGeneticAlgorithm::Population
{
	std::vector<UICodeSoltPopulation> generateInitialPopulations(std::vector<UICodeSolt> initialSolution, 
		UIntSolt2SoltFitenessPair * solt2ScoreTransFun, unsigned int populationNum)
	{
		std::vector<UICodeSoltFitnessPair> pairs(solt2ScoreTransFun(initialSolution));
		return std::vector<UICodeSoltPopulation>(populationNum, UICodeSoltPopulation{ pairs, pairs[0] });
	}

	void run(std::vector<UICodeSoltPopulation>& populations, CompareFun * compareFun, UIntSolt2SoltFitenessPair * solt2ScoreTransFun, const SettingHelper & setting)
	{
		std::vector<std::thread> threads;

		for (UICodeSoltPopulation &population : populations)
			threads.push_back(std::thread(populationOperation, &population, compareFun, solt2ScoreTransFun, setting));

		for (std::thread &thread : threads)
			thread.join();

		populationComunication(populations, compareFun, setting);

		return;
	}

	void populationOperation(UICodeSoltPopulation * population, CompareFun * compareFun, 
		UIntSolt2SoltFitenessPair * solt2ScoreTransFun, const SettingHelper & setting)
	{
		Select::run(population->pairs, compareFun, setting);
		std::vector<UICodeSoltFitnessPair> &thisGenerationPairs(population->pairs);

		do 
		{
			std::vector<UICodeSoltFitnessPair> newGenerationPairs(solt2ScoreTransFun(
				Cross::run(thisGenerationPairs, setting)));

			if (compareFun(newGenerationPairs[0], population->best))
				population->best = newGenerationPairs[0];

			std::vector<UICodeSoltFitnessPair> newMutationGenerationPairs(solt2ScoreTransFun(
				Mutation::run(thisGenerationPairs, setting)));

			if (compareFun(newMutationGenerationPairs[0], population->best))
				population->best = newMutationGenerationPairs[0];

			for (UICodeSoltFitnessPair &pair : newGenerationPairs)
				thisGenerationPairs.emplace_back(pair);
			for (UICodeSoltFitnessPair &pair : newMutationGenerationPairs)
				thisGenerationPairs.emplace_back(pair);

			std::sort(thisGenerationPairs.begin(), thisGenerationPairs.end(), compareFun);

		} while (thisGenerationPairs.size() <= setting.individualNumber.first);
	}

	void populationComunication(std::vector<UICodeSoltPopulation>& populations, CompareFun * compareFun, const SettingHelper & setting)
	{
		if (populations.size() != 1)
		{
			std::map<unsigned int, Comunication::Fun *>::const_iterator currIt(Comunication::funs.find(setting.populationComunicationMode.toUInt()));
			if (currIt != Comunication::funs.cend())
				currIt->second(populations, compareFun);
			else
				Comunication::defaultFun(populations, compareFun);

			for (UICodeSoltPopulation & population : populations)
			{
				std::sort(population.pairs.begin(), population.pairs.end(), compareFun);

				if (compareFun(population.pairs[0], population.best))
					population.best = population.pairs[0];
			}
		}
	}

	namespace Comunication::Funs
	{
		void RandomFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun)
		{
			static std::random_device rd;
			static std::mt19937_64 gen(rd());
			std::poisson_distribution<> d(populations.size());

			unsigned int exchangeTime(d(gen));

			for (unsigned int i(0); i != exchangeTime; ++i)
			{
				unsigned int m(rd() % populations.size()), n(rd() % populations.size());
				UICodeSoltPopulation &populationA(populations[m]);
				UICodeSoltPopulation &populationB(populations[n]);

				unsigned int exchangeNum(d(gen));

				for (unsigned j(0); j != exchangeNum; ++j)
				{
					unsigned int p(rd() % populationA.pairs.size()), q(rd() % populationB.pairs.size());
					populationA.pairs.push_back(populationB.pairs[q]);
					populationB.pairs.push_back(populationA.pairs[p]);
				}
			}
		}

		void BetterToWorseFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun)
		{
			static std::random_device rd;
			static std::mt19937_64 gen(rd());
			std::poisson_distribution<> d(populations.size());

			std::vector<UICodeSoltPopulation *> pPopulations;
			std::sort(pPopulations.begin(), pPopulations.end(), [compareFun]
				(const UICodeSoltPopulation *pLop, const UICodeSoltPopulation *pRop) -> bool
			{
				return compareFun(pLop->best, pRop->best);
			});

			std::vector<unsigned int> orderMap(populations.size() / 2, 0);
			for (unsigned int i(0), j(orderMap.size()); i != j; ++i)
				orderMap[i] = i;
			std::random_shuffle(orderMap.begin(), orderMap.end());

			for (unsigned int i(0), j(orderMap.size()); i != j; ++i)
			{
				UICodeSoltPopulation &populationA(populations[i]);
				UICodeSoltPopulation &populationB(populations[orderMap[i]]);

				unsigned int exchangeNum = d(gen);
				if (exchangeNum > populationA.pairs.size())
					exchangeNum = populationA.pairs.size();

				for (unsigned int k(0); k != exchangeNum; ++k)
					populationB.pairs.push_back(populationA.pairs[k]);
			}
		}

		void MoreToLessFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun)
		{
			static std::random_device rd;
			static std::mt19937_64 gen(rd());
			std::poisson_distribution<> d(populations.size());

			std::vector<UICodeSoltPopulation *> pPopulations;
			std::sort(pPopulations.begin(), pPopulations.end(), []
				(const UICodeSoltPopulation *pLop, const UICodeSoltPopulation *pRop) -> bool
			{
				return pLop->pairs.size() > pRop->pairs.size();
			});

			std::vector<unsigned int> orderMap(populations.size() / 2, 0);
			for (unsigned int i(0), j(orderMap.size()); i != j; ++i)
				orderMap[i] = i;
			std::random_shuffle(orderMap.begin(), orderMap.end());

			for (unsigned int i(0), j(orderMap.size()); i != j; ++i)
			{
				UICodeSoltPopulation &populationA(populations[i]);
				UICodeSoltPopulation &populationB(populations[orderMap[i]]);

				unsigned int exchangeNum = d(gen);
				if (exchangeNum > populationA.pairs.size())
					exchangeNum = populationA.pairs.size();

				for (unsigned int k(0); k != exchangeNum; ++k)
					populationB.pairs.push_back(populationA.pairs[k]);
			}
		}
	}
}