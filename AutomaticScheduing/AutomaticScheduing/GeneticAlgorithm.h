#pragma once

#include "GeneticAlgorithmPoputationOperation.h"

namespace UICodeGeneticAlgorithm
{
	static const unsigned int MaxGeneration = 10;

	UICodeSolt run(const std::vector<UICodeSolt> &initialSolution,
		const SettingHelper &setting, UIntSolt2SoltFitenessPair *solt2ScoreTransFun);
};