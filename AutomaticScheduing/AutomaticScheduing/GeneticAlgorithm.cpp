#include "GeneticAlgorithm.h"
#include <algorithm>
#include <functional>

UICodeGeneticAlgorithm::UICodeSolt UICodeGeneticAlgorithm::run(
	const std::vector<UICodeSolt> &initialSolution, SettingHelper setting, UIntSolt2SoltFitenessPair * solt2ScoreTransFun)
{
	unsigned int generationCounter = 0;
	std::vector<Population::UICodeSoltPopulation> thisGenerationPopulations(
		Population::generateInitialPopulations(initialSolution, solt2ScoreTransFun, setting.populationNum.toUInt() + 1));
	UICodeSoltFitnessPair bestSoltFitnessPair(thisGenerationPopulations[0].best);

	CompareFun *compareFun((setting.fitnessType == FitnessType::BiggerBetter) ? BiggerIsBetter :
		(setting.fitnessType == FitnessType::SmallerBetter) ? SmallerIsBetter : SmallerIsBetter);
	auto betterThanBest(std::bind(compareFun, bestSoltFitnessPair, std::placeholders::_1));

	while (generationCounter != MaxGeneration)
	{
		Population::run(thisGenerationPopulations, compareFun, solt2ScoreTransFun, setting);
		for (const Population::UICodeSoltPopulation &population : thisGenerationPopulations)
		{
			if (betterThanBest(population.best))
			{
				bestSoltFitnessPair = population.best;
				generationCounter = 0;
			}
		}
		++generationCounter;
	}

	return std::move(bestSoltFitnessPair.first);
}