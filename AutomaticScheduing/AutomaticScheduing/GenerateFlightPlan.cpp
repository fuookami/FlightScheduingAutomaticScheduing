#include "GenerateFlightPlan.h"

#include <fstream>
#include <thread>

void GenerateFlightPlan::run()
{
	SubFun::loadDatas();
	std::vector<PlanTable> initialSolution(SubFun::generateInitialSolution());

	// use genetic algorithm to get solution
}

void GenerateFlightPlan::SubFun::loadDatas(void)
{
	std::ifstream fin(dataInputFileName);
	fin >> GenerateFlightPlan::FlighterNum;
	std::string lineData;
	getline(fin, lineData);
	while (std::getline(fin, lineData))
	{
		std::shared_ptr<FlightInfo> newFlightInfo(new FlightInfo(flightInfoSet.size(), lineData));
		flightInfoSet.insert(newFlightInfo);
		flightInfoMap.insert(std::make_pair(newFlightInfo->id, newFlightInfo));
	}

	FlightPlan::setFlighterNum(GenerateFlightPlan::FlighterNum);
	FlightPlan::setFlightInfoNum(flightInfoSet.size());
}

std::vector<PlanTable> GenerateFlightPlan::SubFun::generateInitialSolution(void)
{
	std::vector<PlanTable> initialSolution(GenerateFlightPlan::FlighterNum, PlanTable());
	std::vector<std::thread> threads;

	for (unsigned int i(0), j(initialSolution.size()); i != j; ++i)
		threads.push_back(std::thread(
			FlightPlan::generatePlanTableWithRandomGreedyAlgorithm, &(initialSolution[i]), flightInfoMap));

	for (auto &thread : threads)
		thread.join();

	return std::move(initialSolution);
}