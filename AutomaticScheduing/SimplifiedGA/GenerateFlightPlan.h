#pragma once

#include "FlightPlan.h"

using Solution = PlanTable;
using SolutionWithScore = PlanTableWithScore;

namespace GenerateFlightPlan
{
	extern unsigned int FlighterNum;
	extern unsigned int FlightInfoNum;
	extern unsigned int InitailSolutionNum;

	extern FlightInfoSet flightInfoSet;
	extern FlightInfoMap flightInfoMap;

	static const std::string dataOuputFilter = "result\\";

	struct OutputDatas
	{
		std::vector<std::pair<unsigned int, unsigned int>> minAndMaxScoresOfIters;
		std::vector<std::pair<unsigned int, unsigned int>> minAndMaxPopulationQuantityOfIters;
		std::vector<double> mutationRateOfIters;
		PlanTableWithScore bestPair;
	};

	void loadDatas(const std::string &dataInputFileName);

	namespace SubFun 
	{
		std::vector<PlanTable> generateInitailSolution(void);
		std::vector<std::pair<PlanTable, unsigned int>> planTable2Score(const std::vector<PlanTable> &planTables, bool FaultTolerant = true);
		bool ComparePlanTable(const std::pair<PlanTable, unsigned int> &lop, const std::pair<PlanTable, unsigned int> &rop);
		void outputDatas(const OutputDatas &datas, const std::string &dataOutputFileName);
	};

	using SolutionScoreFunction_t = decltype(&SubFun::planTable2Score);
	using SolutionCompareFunciont_t = decltype(&SubFun::ComparePlanTable);
	using SolveFunction_t = OutputDatas(*)(const std::vector<PlanTable> &initialSolution, bool FaultToTerant, 
		const std::pair<unsigned int, unsigned int> &_solutionNumRange, const std::pair<unsigned int, unsigned int> &range,
		SolutionScoreFunction_t toScoreFun, SolutionCompareFunciont_t compareFun);

	void run(bool FaultToTerant, SolveFunction_t solveFun, const std::pair<double, double> &solutionNumRate, const std::string &dataOutputFileName);
	void testInitialSolution(const unsigned int time);
};