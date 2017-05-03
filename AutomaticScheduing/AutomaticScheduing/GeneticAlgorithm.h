#pragma once

#include "GeneticAlgorithmPoputationOperation.h"

namespace UICodeGeneticAlgorithm
{
	static const unsigned int MaxGeneration = 10;

	UICodeSolt run(std::vector<UICodeSolt> initialSolution,
		SettingHelper setting, UIntSolt2SoltFitenessPair *solt2ScoreTransFun);

	namespace Select
	{
		std::vector<UICodeSoltFitnessPair> run(std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};

	namespace Cross
	{
		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};

	namespace Mutation
	{
		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};
};