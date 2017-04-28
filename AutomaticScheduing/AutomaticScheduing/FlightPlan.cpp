#include "FlightPlan.h"

#include <algorithm>

void FlightPlan::setFlighterNums(const unsigned int i)
{
	orgBunches.insert(orgBunches.begin(), i, FlightBunch());
}

void FlightPlan::generatePlanTableWithRandomGreedyAlgorithm(PlanTable & ret, const FlightInfoMap & infoMap)
{
	// to do
}

std::shared_ptr<FlightPlan> FlightPlan::generateFromPlanTable(PlanTable & t, const FlightInfoMap & infoMap)
{
	std::shared_ptr<FlightPlan> pNewPlan(new FlightPlan());
	for (unsigned int i(0), j(t.size()); i != j; ++i)
	{
		if (!pNewPlan->bunches[t[i]].addFlight(infoMap.find(i)->second))
			return nullptr;
	}

	for (const FlightBunch &bunch : pNewPlan->bunches)
		pNewPlan->totalDelay += bunch.delay();

	return pNewPlan;
}

std::shared_ptr<FlightPlan> FlightPlan::generateFromPlanTableWithFaultTolerant(PlanTable & t, const FlightInfoMap & infoMap)
{
	std::shared_ptr<FlightPlan> pNewPlan(new FlightPlan());
	std::vector<std::pair<unsigned int, std::vector<std::pair<unsigned int, Time>>>> addedDelayTable;
	for (unsigned int i(0), j(t.size()); i != j; ++i)
	{
		if (!pNewPlan->bunches[t[i]].addFlight(infoMap.find(i)->second))
			addedDelayTable.push_back(std::make_pair(i, std::vector<std::pair<unsigned int, Time>>()));
	}

	for (std::pair<unsigned int, std::vector<std::pair<unsigned int, Time>>> &line : addedDelayTable)
	{
		for (unsigned int i(0), j(pNewPlan->bunches.size()); i != j; ++i)
		{
			const FlightBunch &bunch(pNewPlan->bunches[i]);
			Time thisAddedDelay(bunch.addedDelayIfAddFlight(infoMap.find(line.first)->second));
			if (thisAddedDelay != SpecialTime::MaxTime)
				line.second.emplace_back(std::make_pair(i, std::move(thisAddedDelay)));
		}
		std::sort(line.second.begin(), line.second.end(), [](const std::pair<unsigned int, Time> &lop, 
			const std::pair<unsigned int, Time> &rop)->bool
		{
			return lop.second < rop.second;
		});
	}

	if (std::find_if(addedDelayTable.cbegin(), addedDelayTable.cend(), 
		[](const std::pair<unsigned int, std::vector<std::pair<unsigned int, Time>>> &line)->bool
	{
		return line.second.empty();
	}) != addedDelayTable.cend())
	{
		return nullptr;
	}

	for (const std::pair<unsigned int, std::vector<std::pair<unsigned int, Time>>> &line : addedDelayTable)
	{
		bool flag(true);
		for (unsigned int i(0), j(line.second.size()); i != j && flag; ++i)
		{
			if (!pNewPlan->bunches[line.second[i].first].addFlight(infoMap.find(line.first)->second))
			{
				flag = false;
				t[line.first] = line.second[i].first;
			}
		}

		if (flag)
			return nullptr;
	}

	return pNewPlan;
}

const Time &FlightPlan::delay(void) const
{
	return totalDelay;
}

FlightPlan::FlightPlan()
	: bunches(orgBunches), totalDelay(Time(0, 0))
{

}
