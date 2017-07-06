#pragma once

#include "FlightBunch.h"
#include <vector>

using PlanTable = std::vector<unsigned int>;

class FlightPlan
{
public:
	static void setFlighterNum(const unsigned int i);
	static void setFlightInfoNum(const unsigned int i);
	static void generatePlanTableWithRandomGreedyAlgorithm(PlanTable * pRet, FlightInfoMap &infoMap);
	static std::shared_ptr<FlightPlan> generateFromPlanTable(const PlanTable &t, FlightInfoMap &infoMap);
	static std::shared_ptr<FlightPlan> generateFromPlanTableWithFaultTolerant(PlanTable &t, FlightInfoMap &infoMap);
	
	PlanTable getPlanTable(void) const;
	const Time &delay(void) const;

private:
	FlightPlan();

private:
	static std::vector<FlightBunch> orgBunches;
	static PlanTable orgPlanTable;
	std::vector<FlightBunch> bunches;
	Time totalDelay;
};