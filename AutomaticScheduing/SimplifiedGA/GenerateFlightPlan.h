#pragma once

#include "FlightPlan.h"

namespace GenerateFlightPlan
{
	extern unsigned int FlighterNum;
	extern unsigned int FlightInfoNum;

	extern FlightInfoSet flightInfoSet;
	extern FlightInfoMap flightInfoMap;

	static const std::string dataOuputFilter = "result\\";

	struct OutputDatas
	{
		std::vector<std::pair<unsigned int, unsigned int>> minAndMaxScoresOfIters;
		std::vector<std::pair<unsigned int, unsigned int>> minAndMaxPopulationQuantityOfIters;
		std::vector<double> mutationRateOfIters;
		std::pair<PlanTable, unsigned int> bestPair;
	};

	void loadDatas(const std::string &dataInputFileName);

	namespace SubFun 
	{
		std::vector<PlanTable> generateInitialSolution(void);
		std::vector<std::pair<PlanTable, unsigned int>> planTable2Score(const std::vector<PlanTable> &planTables, bool FaultTolerant = true);
		bool ComparePlanTable(const std::pair<PlanTable, unsigned int> &lop, const std::pair<PlanTable, unsigned int> &rop);
		void outputDatas(const OutputDatas &datas, const std::string &dataOutputFileName);
	};

	using PlanTableScoreFunction_t = decltype(&SubFun::planTable2Score);
	using PlanTbaleCompareFunciont_t = decltype(&SubFun::ComparePlanTable);
	using SolveFunction_t = OutputDatas(*)(const std::vector<PlanTable> &initialSolution, bool FaultToTerant, PlanTableScoreFunction_t toScoreFun, PlanTbaleCompareFunciont_t compareFun);

	void run(bool FaultToTerant, SolveFunction_t solveFun, const std::string &dataOutputFileName);
};