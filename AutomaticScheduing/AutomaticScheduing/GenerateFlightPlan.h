#pragma once

#include "FlightPlan.h"
#include "GeneticAlgorithm.h"

namespace GenerateFlightPlan
{
	static unsigned int FlighterNum = 0;
	static unsigned int FlightInfoNum = 0;

	void run();

	namespace SubFun
	{
		static const std::string dataInputFileName = "data.txt";
		static const std::string dataOuputFilter = "result\\";
		FlightInfoSet loadDatas(void);
		std::vector<PlanTable> generateInitialSolution(const FlightInfoSet &flightInfoSet);
	}
}