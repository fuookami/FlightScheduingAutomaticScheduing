#pragma once

#include "FlightPlan.h"
#include "GeneticAlgorithm.h"

namespace GenerateFlightPlan
{
	static unsigned int FlighterNum = 0;
	static unsigned int FlightInfoNum = 0;

	static FlightInfoSet flightInfoSet;
	static FlightInfoMap flightInfoMap;

	void loadDatas(void);
	void run(bool FaultTolerant, const UICodeGeneticAlgorithm::SettingHelper &setting);

	namespace SubFun
	{
		static const std::string dataInputFileName = "data.txt";
		static const std::string dataOuputFilter = "result\\";
		std::vector<PlanTable> generateInitialSolution(void);

		std::vector<std::pair<PlanTable, unsigned int>> planTable2Score(const std::vector<PlanTable> &planTables);
		std::vector<std::pair<PlanTable, unsigned int>> planTable2ScoreWithFaultTolerant(const std::vector<PlanTable> &planTables);
	}
}