#include "FlightPlan.h"

#include <algorithm>
#include <random>

std::vector<FlightBunch> FlightPlan::orgBunches = std::vector<FlightBunch>();
PlanTable FlightPlan::orgPlanTable = PlanTable();

void FlightPlan::setFlighterNum(const unsigned int i)
{
	orgBunches.insert(orgBunches.begin(), i, FlightBunch());
}

void FlightPlan::setFlightInfoNum(const unsigned int i)
{
	orgPlanTable.insert(orgPlanTable.begin(), i, 0);
}

void FlightPlan::generatePlanTableWithRandomGreedyAlgorithm(PlanTable * pRet, const FlightInfoMap & infoMap)
{
	static auto randomRank([]()->unsigned int {
		static unsigned int maxRank = 3;;
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::poisson_distribution<> d(maxRank - 1);

		unsigned int currNum(d(gen) / maxRank);
		return currNum < maxRank ? currNum : maxRank - 1;
	});

	std::shared_ptr<FlightPlan> pNewPlan(nullptr);
	bool flag(true);
	while (flag)
	{
		pNewPlan.reset(new FlightPlan());
		flag = false;

		for (const std::pair<unsigned int, std::shared_ptr<FlightInfo>> &infoPair : infoMap)
		{
			std::vector<std::pair<unsigned int, Time>> addedDelays;

			for (unsigned int i(0), j(pNewPlan->bunches.size()); i != j
				&& pNewPlan->bunches[i].size() != 0; ++i)
			{
				Time thisDelay(pNewPlan->bunches[i].addedDelayIfAddFlight(infoPair.second));
				if (thisDelay != SpecialTime::MaxTime)
					addedDelays.push_back(std::make_pair(i, std::move(thisDelay)));
			}

			if (addedDelays.empty() && pNewPlan->bunches.back().size() != 0)
			{
				flag = true;
				break;
			}
			else if (!addedDelays.empty())
			{
				std::sort(addedDelays.begin(), addedDelays.end(),
					[](std::pair<unsigned int, Time> &lop, std::pair<unsigned int, Time> &rop)->bool
				{
					return lop.second < rop.second;
				});

				unsigned int currRank(randomRank());
				currRank = currRank >= addedDelays.size() ? addedDelays.size() - 1 : currRank;

				pNewPlan->bunches[addedDelays[currRank].first].addFlight(infoPair.second);
			}
			else 
			{
				unsigned int i(0);
				for (unsigned int j(pNewPlan->bunches.size()); i != j && pNewPlan->bunches[i].size() != 0; ++i);
				pNewPlan->bunches[i].addFlight(infoPair.second);
			}
		}
	}

	*pRet = std::move(pNewPlan->getPlanTable());
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

PlanTable FlightPlan::getPlanTable(void) const
{
	PlanTable ret(orgPlanTable);

	for (unsigned int i(0), j(bunches.size()); i != j; ++i)
	{
		for (unsigned int m(0), n(bunches[i].size()); m != n; ++m)
		{
			ret[bunches[i][m].info().id] = i;
		}
	}

	return std::move(ret);
}

const Time &FlightPlan::delay(void) const
{
	return totalDelay;
}

FlightPlan::FlightPlan()
	: bunches(orgBunches), totalDelay(Time(0, 0))
{

}
