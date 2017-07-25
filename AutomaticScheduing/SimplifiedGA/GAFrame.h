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
	static std::pair<unsigned int, unsigned int> range;

	GenerateFlightPlan::OutputDatas run(const std::vector<Solution> &initialSolution, bool FaultToTerant, 
		std::pair<unsigned int, unsigned int> _range, GenerateFlightPlan::SolutionScoreFunction_t toScoreFun, 
		GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
	
	namespace Sub
	{
		void init(const std::vector<Solution> &initialSolution, std::pair<unsigned int, unsigned int> range);
		bool refreshOutputs(GenerateFlightPlan::OutputDatas &output, std::vector<std::shared_ptr<GA::Population::Population>> &populations,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
	};

	namespace Population
	{
		struct Population
		{
			std::vector<SolutionWithScore> paris;
			SolutionWithScore best;
		};

		static const unsigned int PopulationNum = 4;

		std::vector<std::shared_ptr<Population>> generateInitialPopulation(const std::vector<Solution> &initialSolution, bool FaultToTerant,
			GenerateFlightPlan::SolutionScoreFunction_t toScoreFun);
		void run(std::vector<std::shared_ptr<Population>> &populations, GenerateFlightPlan::SolutionScoreFunction_t toScoreFun, bool FaultToTerant,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
		void iteration(Population *pPopulation, GenerateFlightPlan::SolutionScoreFunction_t toScoreFun, bool FaultToTerant,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
		void comunicate(std::vector<std::shared_ptr<Population>> &populations, GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
	};

	namespace Select
	{
		void run(std::vector<SolutionWithScore> &pairs, GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
		std::vector<bool> select(const std::vector<SolutionWithScore> &pairs, GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
		
		static const double rate = 0.2;
		unsigned int getCurrIterSolutionNum(unsigned int currSolutionNum);

		namespace Operator
		{
			std::vector<bool> Tournament(const std::vector<SolutionWithScore> &pairs, unsigned int targetNum, GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
		};
	};

	namespace Cross
	{
		std::vector<Solution> run(const std::vector<SolutionWithScore> &pairs);
		static const unsigned int parentNum = 4;
		std::vector<Solution> cross(const std::vector<SolutionWithScore> &pairs);

		namespace Operator
		{
			std::vector<bool> MultiPoint(const unsigned int size);
		};
	};

	namespace Mutation
	{
		std::vector<Solution> run(const std::vector<SolutionWithScore> &pairs);
		Solution mutate(const Solution &solt, const double rate);

		static const double k = 2.5;
		static const double b = .4;
		static const double startMutationRate = .1;
		static double currMutationRate = startMutationRate;
		double getCurrIterMutationRate(void);

		namespace Operator
		{
			std::vector<unsigned int> Gaussian(const unsigned int size, const double rate);
		};
	};
};