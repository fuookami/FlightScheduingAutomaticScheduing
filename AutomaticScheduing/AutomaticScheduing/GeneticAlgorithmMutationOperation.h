#pragma once

#include "GeneticAlgorithmPublic.h"

namespace UICodeGeneticAlgorithm
{
	namespace Mutation
	{
		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};
}