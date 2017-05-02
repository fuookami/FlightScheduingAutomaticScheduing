#include "GenerateFlightPlan.h"

#include <fstream>
#include <thread>
#include <algorithm>

void GenerateFlightPlan::run(UICodeGeneticAlgorithm::SettingHelper setting)
{
	SubFun::loadDatas();
	std::vector<PlanTable> initialSolution(SubFun::generateInitialSolution());

	UICodeGeneticAlgorithm::run(initialSolution, setting, &SubFun::planTable2Score);
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

std::vector<std::pair<PlanTable, unsigned int>> GenerateFlightPlan::SubFun::planTable2Score(const std::vector<PlanTable> &planTables)
{
	std::vector<std::pair<PlanTable, unsigned int>> ret;

	for (unsigned int i(0), j(planTables.size()); i != j; ++i)
	{
		std::shared_ptr<FlightPlan> flightPlan(FlightPlan::generateFromPlanTable(planTables[i], flightInfoMap));
		if (flightPlan != nullptr)
			ret.emplace_back(std::make_pair(planTables[i], flightPlan->delay().totalMins()));
	}

	std::sort(ret.begin(), ret.end(), [](const std::pair<PlanTable, unsigned int> &lop,
		const std::pair<PlanTable, unsigned int> &rop) -> bool
	{
		return lop.second < rop.second;
	});

	return std::move(ret);
}

std::vector<std::pair<PlanTable, unsigned int>> GenerateFlightPlan::SubFun::planTable2Score(const std::vector<PlanTable> &planTables)
{
	std::vector<std::pair<PlanTable, unsigned int>> ret;

	for (unsigned int i(0), j(planTables.size()); i != j; ++i)
	{
		PlanTable thisPlanTable(planTables[i]);
		std::shared_ptr<FlightPlan> flightPlan(
			FlightPlan::generateFromPlanTableWithFaultTolerant(thisPlanTable, flightInfoMap));
		if (flightPlan != nullptr)
			ret.emplace_back(std::make_pair(thisPlanTable, flightPlan->delay().totalMins()));
	}

	std::sort(ret.begin(), ret.end(), [](const std::pair<PlanTable, unsigned int> &lop,
		const std::pair<PlanTable, unsigned int> &rop) -> bool
	{
		return lop.second < rop.second;
	});

	for (std::pair<PlanTable, unsigned int> &p : ret)
		p.second = ret.back().second - p.second;

	return std::move(ret);
}