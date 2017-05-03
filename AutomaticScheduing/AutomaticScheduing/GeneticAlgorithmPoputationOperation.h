#pragma once

#include "GeneticAlgorithmPublic.h"

namespace UICodeGeneticAlgorithm
{
	namespace Population
	{
		using UICodeSoltPopulation = struct
		{
			std::vector<UICodeSoltFitnessPair> pairs;
			UICodeSoltFitnessPair best;
		};

		std::vector<UICodeSoltPopulation> generateInitialPopulations(const std::vector<UICodeSolt> initialSolution, UIntSolt2SoltFitenessPair * solt2ScoreTransFun, unsigned int populationNum);
		std::vector<UICodeSoltPopulation> run(const std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun, const SettingHelper &setting);
		void populationOperation(UICodeSoltPopulation *populations, CompareFun *compareFun, const SettingHelper &setting);
		void populationComunication(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun, const SettingHelper &setting);

		namespace Comunication
		{
			using Fun = void(*)(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun);
			std::map<>

				namespace Funs
			{

			};
		}
	}
}