#pragma once

#include "GenerateFlightPlan.h"

namespace GA
{
	using Setting = struct SettingStr
	{
		SettingStr(const std::pair<unsigned int, unsigned int> &_solutionNumRange,
			const std::pair<unsigned int, unsigned int> & _range)
			: iter(0), bestContinueIter(0), currMaxSolutionNum(_solutionNumRange.first),
			solutionNumRange(_solutionNumRange), range(_range) {}

		unsigned int iter = 0;
		unsigned int bestContinueIter = 0;
		unsigned int currMaxSolutionNum = 0;
		std::pair<unsigned int, unsigned int> solutionNumRange = std::make_pair(0, 0);
		std::pair<unsigned int, unsigned int> range = std::make_pair(0, 0);
	};

	using PopulationData = struct
	{
		std::vector<SolutionWithScore> paris;
		SolutionWithScore best;
	};

	static const unsigned int maxBestContinueIter = 30;
	static const unsigned int maxIter = 500;
	static const std::pair<double, double> solutionNumRate(std::make_pair(0.75, 2));

	GenerateFlightPlan::OutputDatas run(const std::vector<Solution> &initialSolution, bool FaultToTerant, 
		const std::pair<unsigned int, unsigned int> &_solutionNumRange, const std::pair<unsigned int, unsigned int> &_range, 
		GenerateFlightPlan::SolutionScoreFunction_t toScoreFun,
		GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
	
	namespace Sub
	{
		bool refreshOutputs(GenerateFlightPlan::OutputDatas &output, std::vector<std::shared_ptr<PopulationData>> &populations,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting);
		void printCurrIter(std::ofstream &fout, const GenerateFlightPlan::OutputDatas &output, const Setting &setting);
	};

	namespace Population
	{
		static const unsigned int PopulationNum = 4;

		std::vector<std::shared_ptr<PopulationData>> generateInitialPopulation(const std::vector<Solution> &initialSolution, bool FaultToTerant,
			GenerateFlightPlan::SolutionScoreFunction_t toScoreFun);
		void run(std::vector<std::shared_ptr<PopulationData>> &populations, GenerateFlightPlan::SolutionScoreFunction_t toScoreFun, bool FaultToTerant,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting);
		void iteration(PopulationData *pPopulation, GenerateFlightPlan::SolutionScoreFunction_t toScoreFun, bool FaultToTerant,
			GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting);
		void comunicate(std::vector<std::shared_ptr<PopulationData>> &populations, GenerateFlightPlan::SolutionCompareFunciont_t compareFun);
	};

	namespace Select
	{
		void run(std::vector<SolutionWithScore> &pairs, GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting);
		std::vector<bool> select(const std::vector<SolutionWithScore> &pairs, GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting);
		
		static const double rate = 0.2;
		unsigned int getCurrIterSolutionNum(unsigned int currSolutionNum, const Setting &setting);

		namespace Operator
		{
			std::vector<bool> Tournament(const std::vector<SolutionWithScore> &pairs, unsigned int targetNum, GenerateFlightPlan::SolutionCompareFunciont_t compareFun, const Setting &setting);
		};
	};

	namespace Cross
	{
		std::vector<Solution> run(const std::vector<SolutionWithScore> &pairs, const Setting &setting);
		static const unsigned int parentNum = 4;
		void cross(std::vector<Solution> &pairs);

		namespace Operator
		{
			std::vector<bool> MultiPoint(const unsigned int size);
		};
	};

	namespace Mutation
	{
		std::vector<Solution> run(const std::vector<SolutionWithScore> &pairs, const Setting &setting);
		Solution mutate(const Solution &solt, const double rate, const Setting &setting);

		static const double k = 2.5;
		static const double b = .4;
		static const double startMutationRate = .1;
		static double currMutationRate = startMutationRate;
		double getCurrIterMutationRate(const Setting &setting);

		namespace Operator
		{
			std::vector<unsigned int> Gaussian(const unsigned int size, const double rate);
		};
	};
};