#include "GeneticAlgorithmPoputationOperation.h"

#include <thread>

namespace UICodeGeneticAlgorithm::Population
{
	std::vector<UICodeSoltPopulation> generateInitialPopulations(
		const std::vector<UICodeSolt> initialSolution, UIntSolt2SoltFitenessPair * solt2ScoreTransFun, unsigned int populationNum)
	{
		std::vector<UICodeSoltFitnessPair> pairs(solt2ScoreTransFun(initialSolution));
		return std::vector<UICodeSoltPopulation>(populationNum, UICodeSoltPopulation{ pairs, pairs[0] });
	}

	void run(std::vector<UICodeSoltPopulation>& populations, CompareFun * compareFun, const SettingHelper & setting)
	{
		std::vector<std::thread> threads;

		for (UICodeSoltPopulation &population : populations)
			threads.push_back(std::thread(populationOperation, &population, compareFun, setting));

		for (std::thread &thread : threads)
			thread.join();

		populationComunication(populations, compareFun, setting);

		return;
	}

	void populationOperation(UICodeSoltPopulation * populations, CompareFun * compareFun, const SettingHelper & setting)
	{
		std::vector<UICodeSoltFitnessPair> thisGenerationPairs(Select::run(populations->pairs, setting));
	}

	void populationComunication(std::vector<UICodeSoltPopulation>& populations, CompareFun * compareFun, const SettingHelper & setting)
	{
		if (populations.size() != 1)
		{
			std::map<unsigned int, Comunication::Fun>::const_iterator currIt(Comunication::funs.find(setting.populationComunicationMode.toUInt()));
			if (currIt != Comunication::funs.cend())
				currIt->second(populations, compareFun);
			else
				Comunication::defaultFun(populations, compareFun);
		}
	}

	namespace Comunication::Funs
	{
		void RandomFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun)
		{

		}

		void BetterToWorseFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun)
		{

		}

		void MoreToLessFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun)
		{

		}
	}
}