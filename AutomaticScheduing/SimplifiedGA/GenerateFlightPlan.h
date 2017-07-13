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
		std::vector<std::pair<unsigned int, unsigned int>> secrosOfIters;
		std::pair<PlanTable, unsigned int> bestPair;
	};

	void loadDatas(const std::string &dataInputFileName);

	namespace SubFun 
	{
		std::vector<PlanTable> generateInitialSolution(void);
		std::vector<std::pair<PlanTable, unsigned int>> planTable2Score(const std::vector<PlanTable> &planTables, bool FaultTolerant = true);

		void outputDatas(const OutputDatas &datas, const std::string &dataOutputFileName);
	};

	using PlanTableScoreFunction_t = decltype(&SubFun::planTable2Score);
	using SolveFunction_t = OutputDatas(*)(const std::vector<PlanTable> &initialSolution, bool FaultToTerant, PlanTableScoreFunction_t toScoreFun);

	void run(bool FaultToTerant, SolveFunction_t solveFun, const std::string &dataOutputFileName);
};