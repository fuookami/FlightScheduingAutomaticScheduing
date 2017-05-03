#pragma once

#include "GeneticAlgorithmPoputationOperation.h"

namespace UICodeGeneticAlgorithm
{
	static const unsigned int MaxGeneration = 10;

	UICodeSolt run(std::vector<UICodeSolt> initialSolution,
		SettingHelper setting, UIntSolt2SoltFitenessPair *solt2ScoreTransFun);
};