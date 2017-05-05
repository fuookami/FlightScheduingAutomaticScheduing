#pragma once

#include "GeneticAlgorithmPublic.h"

namespace UICodeGeneticAlgorithm
{
	namespace Cross
	{
		static const unsigned int minMultiParentNum = 3;
		static const unsigned int maxMultiParentNum = 6;

		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
		void cross(std::vector<UICodeSolt> *solts, const SettingHelper &setting);

		namespace Mode
		{
			unsigned int getParentNumber(const std::vector<UICodeSoltFitnessPair>& pairs, const SettingHelper &setting);
		}

		namespace Operator
		{
			namespace Funs
			{
				void OnePointFun(std::vector<UICodeSolt> &solt);
				void TwoPointFun(std::vector<UICodeSolt> &solt);
				void MultiPointFun(std::vector<UICodeSolt> &solt);
				void UniformFun(std::vector<UICodeSolt> &solt);
				void CycleFun(std::vector<UICodeSolt> &solt);
			};

			using Fun = void(std::vector<UICodeSolt> &solt);
			static Fun *defaultFun = Funs::MultiPointFun;
			static const std::map<unsigned int, Fun *> funs =
			{
				std::make_pair(CrossOperator::OnePoint,		Funs::OnePointFun),
				std::make_pair(CrossOperator::TwoPoint,		Funs::TwoPointFun),
				std::make_pair(CrossOperator::MultiPoint,	Funs::MultiPointFun),
				std::make_pair(CrossOperator::Uniform,		Funs::UniformFun),
				std::make_pair(CrossOperator::Cycle,		Funs::CycleFun)
			};

			void CrossAreas(std::vector<UICodeSolt> &solt, const std::vector<bool> &mark);
		};
	};
};