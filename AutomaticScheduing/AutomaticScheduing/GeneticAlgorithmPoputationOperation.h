#pragma once

#include "GeneticAlgorithmPublic.h"
#include "GeneticAlgorithmSelectOperation.h"
#include "GeneticAlgorithmCrossOperation.h"
#include "GeneticAlgorithmMutationOperation.h"

namespace UICodeGeneticAlgorithm::Population
{
	static const unsigned int MaxPopulationNumber = 8;

	using UICodeSoltPopulation = struct
	{
		std::vector<UICodeSoltFitnessPair> pairs;
		UICodeSoltFitnessPair best;
	};

	std::vector<UICodeSoltPopulation> generateInitialPopulations(std::vector<UICodeSolt> initialSolution, UIntSolt2SoltFitenessPair * solt2ScoreTransFun, unsigned int populationNum);
	void run(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun, UIntSolt2SoltFitenessPair * solt2ScoreTransFun, const SettingHelper &setting);
	void populationOperation(UICodeSoltPopulation *populations, CompareFun *compareFun, UIntSolt2SoltFitenessPair * solt2ScoreTransFun, const SettingHelper &setting);
	void populationComunication(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun, const SettingHelper &setting);

	namespace Comunication
	{
		using Fun = void(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun);
		static const Fun *defaultFun = Funs::BetterToWorseFun;
		static const std::map<unsigned int, Fun *> funs(
		{
			std::make_pair(PopulationComunicationMode::Random,			Funs::RandomFun),
			std::make_pair(PopulationComunicationMode::BetterToWrose,	Funs::BetterToWorseFun),
			std::make_pair(PopulationComunicationMode::MoreToLess,		Funs::MoreToLessFun)
		});

		namespace Funs
		{
			void RandomFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun);
			void BetterToWorseFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun);
			void MoreToLessFun(std::vector<UICodeSoltPopulation> &populations, CompareFun *compareFun);
		};
	};
};