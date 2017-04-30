#pragma once

#include "FlightPlan.h"
#include "GeneticAlgorithm.h"

namespace GenerateFlightPlan
{
	static unsigned int FlighterNum = 0;
	static unsigned int FlightInfoNum = 0;

	static FlightInfoSet flightInfoSet;
	static FlightInfoMap flightInfoMap;

	void run();

	namespace SubFun
	{
		static const std::string dataInputFileName = "data.txt";
		static const std::string dataOuputFilter = "result\\";
		void loadDatas(void);
		std::vector<PlanTable> generateInitialSolution(void);
	}
}