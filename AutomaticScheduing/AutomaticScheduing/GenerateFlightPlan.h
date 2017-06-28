#pragma once

#include "FlightPlan.h"
#include "GeneticAlgorithm.h"

namespace GenerateFlightPlan
{
	extern unsigned int FlighterNum;
	extern unsigned int FlightInfoNum;

	extern FlightInfoSet flightInfoSet;
	extern FlightInfoMap flightInfoMap;

	void loadDatas(void);
	void run(bool FaultTolerant, const UICodeGeneticAlgorithm::SettingHelper &setting);

	namespace SubFun
	{
		static const std::string dataInputFileName = "data2.txt";
		static const std::string dataOuputFilter = "result\\";
		std::vector<PlanTable> generateInitialSolution(void);

		std::vector<std::pair<PlanTable, unsigned int>> planTable2Score(const std::vector<PlanTable> &planTables);
		std::vector<std::pair<PlanTable, unsigned int>> planTable2ScoreWithFaultTolerant(const std::vector<PlanTable> &planTables);
	}
}