#include "GenerateFlightPlan.h"

#include <fstream>
#include <thread>
#include <algorithm>

void GenerateFlightPlan::loadDatas(void)
{
	std::ifstream fin(SubFun::dataInputFileName);
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

void GenerateFlightPlan::run(bool FaultTolerant, UICodeGeneticAlgorithm::SettingHelper setting)
{
	std::vector<PlanTable> initialSolution(SubFun::generateInitialSolution());

	if (FaultTolerant)
		UICodeGeneticAlgorithm::run(initialSolution, setting, SubFun::planTable2ScoreWithFaultTolerant);
	else
		UICodeGeneticAlgorithm::run(initialSolution, setting, SubFun::planTable2Score);
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
	std::vector<std::thread> threads;
	static auto calScore = [](std::pair<PlanTable, unsigned int> *pPair) {
		std::shared_ptr<FlightPlan> flightPlan(FlightPlan::generateFromPlanTable(pPair->first, flightInfoMap));
		if (flightPlan != nullptr)
			pPair->second = flightPlan->delay().totalMins();
		else
			pPair->second = UINT_MAX;
	};

	for (unsigned int i(0), j(planTables.size()); i != j; ++i)
	{
		ret.emplace_back(std::make_pair(planTables[i], 0));
		threads.push_back(std::thread(calScore, &ret.back()));
	}

	for (std::thread &thread : threads)
		thread.join();

	std::sort(ret.begin(), ret.end(), [](const std::pair<PlanTable, unsigned int> &lop,
		const std::pair<PlanTable, unsigned int> &rop) -> bool
	{
		return lop.second < rop.second;
	});
	std::vector<std::pair<PlanTable, unsigned int>>::iterator lastIt(std::find_if(
		ret.begin(), ret.end(), [](const std::pair<PlanTable, unsigned int> &pair) -> bool {
		return pair.second == UINT_MAX;
	}));

	if (lastIt != ret.end())
		ret.erase(lastIt, ret.end());
	return std::move(ret);
}

std::vector<std::pair<PlanTable, unsigned int>> GenerateFlightPlan::SubFun::planTable2ScoreWithFaultTolerant(const std::vector<PlanTable> &planTables)
{
	std::vector<std::pair<PlanTable, unsigned int>> ret;
	std::vector<std::thread> threads;
	static auto calScore = [](std::pair<PlanTable, unsigned int> *pPair) {
		std::shared_ptr<FlightPlan> flightPlan(FlightPlan::generateFromPlanTableWithFaultTolerant(pPair->first, flightInfoMap));
		if (flightPlan != nullptr)
			pPair->second = flightPlan->delay().totalMins();
		else
			pPair->second = UINT_MAX;
	};

	for (unsigned int i(0), j(planTables.size()); i != j; ++i)
	{
		ret.emplace_back(std::make_pair(planTables[i], 0));
		threads.push_back(std::thread(calScore, &ret.back()));
	}

	for (std::thread &thread : threads)
		thread.join();

	std::sort(ret.begin(), ret.end(), [](const std::pair<PlanTable, unsigned int> &lop,
		const std::pair<PlanTable, unsigned int> &rop) -> bool
	{
		return lop.second < rop.second;
	});
	std::vector<std::pair<PlanTable, unsigned int>>::iterator lastIt(std::find_if(
		ret.begin(), ret.end(), [](const std::pair<PlanTable, unsigned int> &pair) -> bool {
		return pair.second == UINT_MAX;
	}));

	if (lastIt != ret.end())
		ret.erase(lastIt, ret.end());
	return std::move(ret);
}