#pragma once

#include "GeneticAlgorithmPublic.h"

namespace UICodeGeneticAlgorithm::Select
{
	static const unsigned int EliteNum = 4;
	static const unsigned int GroupMinNum = 8;

	void run(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const SettingHelper &setting);
	unsigned int getIndividualNum(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	void select(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting);

	namespace Mode
	{
		using Fun = unsigned int(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
		static const Fun *defaultFun = Funs::AdapativeDynamicFun;
		static const std::map<unsigned int, Fun *> funs =
		{
			std::make_pair(SelectMode::Static,				Funs::StaticFun),
			std::make_pair(SelectMode::AdapativeDynamic,	Funs::AdapativeDynamicFun)
		};

		namespace Funs
		{
			unsigned int StaticFun(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
			unsigned int AdapativeDynamicFun(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
		};
	};

	namespace Operator
	{
		using Fun = void(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting);
		static const Fun *defaultFun = Funs::TournamentFun;
		static const std::map<unsigned int, Fun *> funs = 
		{
			std::make_pair(SelectOperator::Roulette,	Funs::RouletteFun),
			std::make_pair(SelectOperator::RankSelect,	Funs::RankSelectFun),
			std::make_pair(SelectOperator::Tournament,	Funs::TournamentFun)
		};

		namespace Funs
		{
			void RouletteFun(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting);
			void RankSelectFun(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting);
			void TournamentFun(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, const unsigned int individualNumber, const SettingHelper &setting);
		};

		void copyPairsToSize(std::vector<UICodeSoltFitnessPair> &pairs, CompareFun *compareFun, unsigned int individualNumber);
		void toDiscreasingOrder(std::vector<unsigned int> &fitensses, const SettingHelper &setting);
	};
}