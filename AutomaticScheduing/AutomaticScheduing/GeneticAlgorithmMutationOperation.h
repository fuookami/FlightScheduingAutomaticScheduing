#pragma once

#include "GeneticAlgorithmPublic.h"

namespace UICodeGeneticAlgorithm
{
	namespace Mutation
	{
		static const double minMutationRate = 0.001;
		static const double maxMutationRate = 0.01;

		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
		void mutate(UICodeSolt **solts, const UICodeSoltFitnessPair &pair, const double rate, const SettingHelper &setting);

		namespace Mode
		{
			double getMutationRate(const std::vector<UICodeSoltFitnessPair>& pairs, const SettingHelper &setting);
		};

		namespace Operator
		{
			namespace Funs
			{
				void UniformFun(UICodeSolt **solt, const double rate, const unsigned int range, const UICodeSoltFitnessPair &pair);
				void NonUniformFun(UICodeSolt **solt, const double rate, const unsigned int range, const UICodeSoltFitnessPair &pair);
				void GaussianFun(UICodeSolt **solt, const double rate, const unsigned int range, const UICodeSoltFitnessPair &pair);
			};

			using Fun = void(UICodeSolt **solt, const double rate, const unsigned int range, const UICodeSoltFitnessPair &pair);
			static Fun *defaultFun = nullptr;
			static const std::map<unsigned int, Fun *> funs =
			{
				std::make_pair(MutationOperator::Uniform,		Funs::UniformFun),
				std::make_pair(MutationOperator::NonUniform,	Funs::NonUniformFun),
				std::make_pair(MutationOperator::Gaussian,		Funs::GaussianFun)
			};

			void MutateArea(UICodeSolt **solt, const std::vector<bool> &mark, const unsigned int range, const UICodeSoltFitnessPair &pair);
		};
	};
};