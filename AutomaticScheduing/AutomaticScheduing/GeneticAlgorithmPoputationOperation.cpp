#include "GeneticAlgorithmPoputationOperation.h"

std::vector<UICodeGeneticAlgorithm::Population::UICodeSoltPopulation> UICodeGeneticAlgorithm::Population::generateInitialPopulations(
	const std::vector<UICodeSolt> initialSolution, UIntSolt2SoltFitenessPair * solt2ScoreTransFun, unsigned int populationNum)
{
	std::vector<UICodeSoltFitnessPair> pairs(solt2ScoreTransFun(initialSolution));
	return std::vector<UICodeSoltPopulation>(populationNum, UICodeSoltPopulation{ pairs, pairs[0] });
}

std::vector<UICodeGeneticAlgorithm::Population::UICodeSoltPopulation> UICodeGeneticAlgorithm::Population::run(
	const std::vector<UICodeSoltPopulation>& populations, CompareFun * compareFun, const SettingHelper & setting)
{

}
