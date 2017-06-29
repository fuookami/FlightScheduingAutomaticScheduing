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
	static unsigned int maxRank = 4;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::poisson_distribution<> d(maxRank - 1);

	static auto randomRank([]()->unsigned int {
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
			else if (!addedDelays.empty() && d(gen) < maxRank)
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

std::shared_ptr<FlightPlan> FlightPlan::generateFromPlanTable(const PlanTable & t, const FlightInfoMap & infoMap)
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
	static unsigned int maxRank = 4;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::poisson_distribution<> d(maxRank - 1);

	std::shared_ptr<FlightPlan> pNewPlan(nullptr);
	bool flag(true);
	while (flag)
	{
		pNewPlan.reset(new FlightPlan());

		std::vector<std::pair<unsigned int, std::vector<std::pair<unsigned int, unsigned int>>>> addedDealyTable;
		for (unsigned int i(0), j(t.size()); i != j; ++i)
		{
			std::shared_ptr<FlightInfo> pThisFlight(infoMap.find(i)->second);

			if (!pNewPlan->bunches[t[i]].addFlight(pThisFlight))
			{
				if (d(gen) < maxRank)
					addedDealyTable.push_back(std::make_pair(i, 
						std::vector<std::pair<unsigned int, unsigned int>>()));
				else
				{
					// 寻找一条空航班串
					unsigned int p(0), q(pNewPlan->bunches.size());
					for (; p != q && pNewPlan->bunches[i].size() != 0; ++p);
					if (p != q)
					{
						// 加入到这条航班串里
						pNewPlan->bunches[p].addFlight(pThisFlight);
						t[i] = p;
					}
					else
					{
						// 加入addedDealyTable表里
						addedDealyTable.push_back(std::make_pair(i, 
							std::vector<std::pair<unsigned int, unsigned int>>()));
					}
				}
			}
		}

		for (std::pair<unsigned int, std::vector<std::pair<unsigned int, unsigned int>>> 
			&ele : addedDealyTable)
		{
			std::shared_ptr<FlightInfo> pThisFlight(infoMap.find(ele.first)->second);

			for (unsigned int i(0), j(pNewPlan->bunches.size()); i != j; ++i)
			{
				if (!pNewPlan->bunches[ele.first].addFlight(pThisFlight))
					ele.second.emplace_back(std::make_pair(i,
						pNewPlan->bunches[ele.first].addedDelayIfAddFlight(pThisFlight)));
			}

			std::sort(ele.second.begin(), ele.second.end(), 
				[](std::pair<unsigned int, unsigned int> &lop, 
					std::pair<unsigned int, unsigned int> &rop) -> bool
			{
				return lop.second < rop.second;
			});
		}

		while (!addedDealyTable.empty())
		{
			/*
			存在于这个向量的航班id表示未加入航班串集中
			*/

			/*
			根据 vector<bunch_id, cost>[0].cost 对 addedDealyTable(<flight_id, vector<bunch_id, cost>>) 进行排序，从小到大
			柏松分布选择一个，将这个弹出addedDealyTable
			柏松分布选择bunch_id或者加入到新的航班串里
			记放入的航班串为new_bunch_id

			for <flight_id, vector<bunch_id, cost>> in addedDealyTable
				bool flag
				if new_bunch_id 存在于 vector<bunch_id, cost> 中
					if bunch.cost(flight) 不是无穷大
						更新cost
					else
						删除
					flag = true
				if flag
					根据cost对vector<bunch_id, cost>进行排序，从小到大
			*/
		}

		for (const FlightBunch &bunch : pNewPlan->bunches)
			pNewPlan->totalDelay += bunch.delay();
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
