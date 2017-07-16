#pragma once

#include "GenerateFlightPlan.h"

namespace GA
{
	static unsigned int iter;
	static unsigned int bestContinueIter;
	static const unsigned int maxBestContinueIter = 30;
	static const unsigned int maxIter = 500;
	static unsigned int minSolutionNum;
	static unsigned int currMaxSolutionNum;
	static unsigned int maxSolutionNum;

	GenerateFlightPlan::OutputDatas run(const std::vector<PlanTable> &initialSolution, bool FaultToTerant, 
		GenerateFlightPlan::PlanTableScoreFunction_t toScoreFun, 
		GenerateFlightPlan::PlanTbaleCompareFunciont_t compareFun);
	void refreshOutputs(GenerateFlightPlan::OutputDatas &output, std::vector<Population::Population> &populations);

	namespace Population
	{
		struct Population
		{
			std::vector<PlanTableWithScore> paris;
			PlanTableWithScore best;
		};

		static const unsigned int PopulationNum = 4;

		std::vector<Population> generateInitialPopulation(const std::vector<PlanTable> &initialSolution, bool FaultToTerant,
			GenerateFlightPlan::PlanTableScoreFunction_t toScoreFun);
		void run(std::vector<Population> &populations, GenerateFlightPlan::PlanTableScoreFunction_t toScoreFun,
			GenerateFlightPlan::PlanTbaleCompareFunciont_t compareFun);
		void comunicate(std::vector<Population> &populations);
	};

	namespace Select
	{
		void run(std::vector<PlanTableWithScore> &pairs, GenerateFlightPlan::PlanTbaleCompareFunciont_t compareFun);
		std::vector<bool> select(const std::vector<PlanTableWithScore> &pairs);
		
		static const double rate = 0.2;
		inline unsigned int getCurrIterSolutionNum(void);

		namespace Operator
		{
			std::vector<bool> Tournament(const std::vector<PlanTableWithScore> &pairs, unsigned int targetNum);
		};
	};

	namespace Cross
	{
		std::vector<PlanTableWithScore> run(const std::vector<PlanTableWithScore> &pairs);
		std::vector<PlanTable> cross(const std::vector<PlanTableWithScore *> &pairs);

		namespace Operator
		{
			std::vector<bool> MultiPoint(const unsigned int size);
		};
	};

	namespace Mutation
	{
		std::vector<PlanTableWithScore> run(const std::vector<PlanTableWithScore> &pairs);
		PlanTable mutate(const PlanTable &solt, const std::pair<unsigned int, unsigned int> range);

		static const double k = 2.5;
		static const double b = .4;
		inline double getCurrIterMutationRate(void);

		namespace Operator
		{
			PlanTable Gaussian(const PlanTable &solt, const double rate, const std::pair<unsigned int, unsigned int> range);
		};
	};
}