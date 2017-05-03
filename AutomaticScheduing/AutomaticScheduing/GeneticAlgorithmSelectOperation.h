#pragma once

#include "GeneticAlgorithmPublic.h"

namespace UICodeGeneticAlgorithm
{
	namespace Select
	{
		std::vector<UICodeSoltFitnessPair> run(std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};
}