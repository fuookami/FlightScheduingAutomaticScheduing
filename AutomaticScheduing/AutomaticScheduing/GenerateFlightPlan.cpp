#include "GenerateFlightPlan.h"

#include <fstream>
#include <thread>

void GenerateFlightPlan::run()
{
	FlightInfoSet flightInfoSet(SubFun::loadDatas());
	std::vector<PlanTable> initialSolution(SubFun::generateInitialSolution(flightInfoSet));

	// use genetic algorithm to get solution
}

FlightInfoSet GenerateFlightPlan::SubFun::loadDatas(void)
{
	FlightInfoSet flightInfoSet;

	std::ifstream fin(dataInputFileName);
	fin >> GenerateFlightPlan::FlighterNum;
	std::string lineData;
	while (std::getline(fin, lineData))
		flightInfoSet.insert(std::shared_ptr<FlightInfo>(
			new FlightInfo(flightInfoSet.size(), lineData)));

	return std::move(flightInfoSet);
}

std::vector<PlanTable> GenerateFlightPlan::SubFun::generateInitialSolution(const FlightInfoSet & flightInfoSet)
{
	// multi-threading use FlightPlan::generatePlanTableWithRandomGreedyAlgorithm to generate initial solution
}