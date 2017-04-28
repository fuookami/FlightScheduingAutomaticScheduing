#pragma once

#include "FlightBunch.h"
#include <vector>

using PlanTable = std::vector<unsigned int>;

class FlightPlan
{
public:
	static void setFlighterNums(const unsigned int i);
	void generatePlanTableWithRandomGreedyAlgorithm(PlanTable &ret, const FlightInfoMap &infoMap);
	static std::shared_ptr<FlightPlan> generateFromPlanTable(PlanTable &t, const FlightInfoMap &infoMap);
	static std::shared_ptr<FlightPlan> generateFromPlanTableWithFaultTolerant(PlanTable &t, const FlightInfoMap &infoMap);
	const Time &delay(void) const;

private:
	FlightPlan();

private:
	static std::vector<FlightBunch> orgBunches;
	std::vector<FlightBunch> bunches;
	Time totalDelay;
};