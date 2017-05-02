#include "GeneticAlgorithm.h"
#include <algorithm>

UICodeGeneticAlgorithm::UICodeSolt UICodeGeneticAlgorithm::run(
	std::vector<UICodeSolt> initialSolution, SettingHelper setting, UIntSolt2SoltFitenessPair * solt2ScoreTransFun)
{
	unsigned int generationCounter = 0;
	std::vector<UICodeSoltFitnessPair> thisSolutionFitnessPairs = solt2ScoreTransFun(initialSolution);
	UICodeSoltFitnessPair bestSolutionFitnessPair(thisSolutionFitnessPairs.front());

	while (generationCounter != MaxGeneration)
	{
		std::vector<UICodeSolt> thisGeneration = Select::run(thisSolutionFitnessPairs, setting) ; // 使用选择算子选出新一代，优胜劣汰
		std::vector<UICodeSolt> newGeneration(Cross::run(thisGeneration, setting)); // 使用交叉算子得到新的一代
		std::vector<UICodeSoltFitnessPair> newGenerationFitnessPairs(solt2ScoreTransFun(newGeneration));

		if (newGenerationFitnessPairs[0].second < bestSolutionFitnessPair.second)
		{
			bestSolutionFitnessPair = newGenerationFitnessPairs[0];
			generationCounter = 0;
		}

		std::vector<UICodeSolt> newGenerationFromMutation(Mutation::run(newGenerationFitnessPairs, setting));
		std::vector<UICodeSoltFitnessPair> newGenerationFromMutationFitnessPairs(solt2ScoreTransFun(newGenerationFromMutation));

		if (newGenerationFromMutationFitnessPairs[0].second < bestSolutionFitnessPair.second)
		{
			bestSolutionFitnessPair = newGenerationFromMutationFitnessPairs[0];
			generationCounter = 0;
		}

		thisSolutionFitnessPairs.emplace_back(newGenerationFromMutationFitnessPairs);
		thisSolutionFitnessPairs.emplace_back(newGenerationFromMutationFitnessPairs);
		std::sort(thisSolutionFitnessPairs.begin(), thisSolutionFitnessPairs.end(),
			[](const UICodeSoltFitnessPair &lop, const UICodeSoltFitnessPair &rop) -> bool
		{
			return lop.second < rop.second;
		});

		++generationCounter;
	}

	return std::move(bestSolutionFitnessPair.first);
}
