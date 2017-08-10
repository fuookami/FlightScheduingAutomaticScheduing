#include "FlightPlan.h"

#include <algorithm>
#include <random>

std::vector<FlightBunch> FlightPlan::orgBunches = std::vector<FlightBunch>();
PlanTable FlightPlan::orgPlanTable = PlanTable();

FlightPlan::~FlightPlan()
{
	m_bunches.clear();
}

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

			for (unsigned int i(0), j(pNewPlan->m_bunches.size()); i != j
				&& pNewPlan->m_bunches[i].size() != 0; ++i)
			{
				Time thisDelay(pNewPlan->m_bunches[i].addedDelayIfAddFlight(infoPair.second));
				if (thisDelay != SpecialTime::MaxTime)
					addedDelays.push_back(std::make_pair(i, thisDelay.totalMins()));
			}

			if (addedDelays.empty() && pNewPlan->m_bunches.back().size() != 0)
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

				pNewPlan->m_bunches[addedDelays[currRank].first].addFlight(infoPair.second);
			}
			else 
			{
				unsigned int i(0), j(pNewPlan->m_bunches.size());
				for (; i != j && pNewPlan->m_bunches[i].size() != 0; ++i);
				if (i == j)
				{
					flag = true;
					break;
				}
				pNewPlan->m_bunches[i].addFlight(infoPair.second);
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
		if (!pNewPlan->m_bunches[t[i]].addFlight(infoMap.find(i)->second))
			return nullptr;
	}

	for (const FlightBunch &bunch : pNewPlan->m_bunches)
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
	PlanTable tCopy;
	
	unsigned int maxTime(maxRank + d(gen));
	maxTime = maxTime > 10 ? 10 : maxTime;
	unsigned int counter(0);
	for (; flag && counter != maxTime; ++counter)
	{
		pNewPlan.reset(new FlightPlan());
		tCopy = t;
		flag = false;

		std::vector<std::pair<unsigned int, std::vector<std::pair<unsigned int, int>>>> addedDealyTable;
		for (unsigned int i(0), j(tCopy.size()); i != j; ++i)
		{
			std::shared_ptr<FlightInfo> pThisFlight(infoMap.find(i)->second);

			if (!pNewPlan->m_bunches[tCopy[i]].addFlight(pThisFlight))
			{
				if (d(gen) < maxRank)
					addedDealyTable.push_back(std::make_pair(i, 
						std::vector<std::pair<unsigned int, int>>()));
				else
				{
					// 寻找一条空航班串
					unsigned int p(0), q(pNewPlan->m_bunches.size());
					for (; p != q && pNewPlan->m_bunches[p].size() != 0; ++p);
					if (p != q)
					{
						// 加入到这条航班串里
						pNewPlan->m_bunches[p].addFlight(pThisFlight);
						tCopy[i] = p;
					}
					else
					{
						// 加入addedDealyTable表里
						addedDealyTable.push_back(std::make_pair(i, 
							std::vector<std::pair<unsigned int, int>>()));
					}
				}
			}
		}

		/*
		for <flight_id, vector<bunch_id, cost>> in addedDealyTable
			for bunch in pNewPlan
				if bunch.cost(flight)不是无穷大
					将<bunch_id, cost>放入addedDealyTable[flight_id]
			if 这个航班有航班串能塞进去
				根据cost对vector<bunch_id, cost>进行排序，从小到大
			else
				寻找一条空航班串
				if 存在新航班
					加入到这条航班里
				else
					重新生成
		*/
		for (std::pair<unsigned int, std::vector<std::pair<unsigned int, int>>> 
			&ele : addedDealyTable)
		{
			std::shared_ptr<FlightInfo> pThisFlight(infoMap.find(ele.first)->second);

			for (unsigned int i(0), j(pNewPlan->m_bunches.size()); i != j; ++i)
			{
				Time cost(pNewPlan->m_bunches[i].addedDelayIfAddFlight(pThisFlight));
				if (cost != SpecialTime::MaxTime)
				{
					ele.second.push_back(std::make_pair(i, cost.totalMins()));
				}
			}

			if (!ele.second.empty())
			{
				std::sort(ele.second.begin(), ele.second.end(),
					[](std::pair<unsigned int, int> &lop,
						std::pair<unsigned int, int> &rop) -> bool
				{
					return lop.second < rop.second;
				});
			}
			else
			{
				unsigned int p(0), q(pNewPlan->m_bunches.size());
				for (; p != q && pNewPlan->m_bunches[p].size() != 0; ++p);
				if (p != q)
				{
					pNewPlan->m_bunches[p].addFlight(pThisFlight);
					tCopy[ele.first] = p;
				}
				else
				{
					flag = true;
					break;
				}
			}
		}

		if (flag)
			continue;

		for (auto bgIt(addedDealyTable.begin()); bgIt != addedDealyTable.end();)
		{
			if (bgIt->second.empty())
				bgIt = addedDealyTable.erase(bgIt);
			else
				++bgIt;
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
			*/
			std::sort(addedDealyTable.begin(), addedDealyTable.end(), []
			(std::pair<unsigned int, std::vector<std::pair<unsigned int, int>>> &lps,
				std::pair<unsigned int, std::vector<std::pair<unsigned int, int>>> &rps) -> bool
			{
				return lps.second.front().second < rps.second.front().second;
			});

			unsigned int selectRank(d(gen));
			selectRank = selectRank >= addedDealyTable.size() ? addedDealyTable.size() - 1 : selectRank;
			std::vector<std::pair<unsigned int, std::vector<std::pair<unsigned int, 
				int>>>>::iterator pSelect(addedDealyTable.begin() + selectRank);

			unsigned int selectBunch(d(gen));
			selectBunch = selectBunch >= pSelect->second.size() ? pSelect->second.size() - 1 : selectBunch;
			unsigned int bunchId(pSelect->second[selectBunch].first);
			pNewPlan->m_bunches[bunchId].addFlight(infoMap.find(pSelect->first)->second);
			tCopy[pSelect->first] = bunchId;
			addedDealyTable.erase(pSelect);

			/*
			更新addedDealyTable表
			for <flight_id, vector<bunch_id, cost>> in addedDealyTable
				bool flag2
				if new_bunch_id 存在于 vector<bunch_id, cost> 中
					if bunch.cost(flight) 不是无穷大
						更新cost
					else
						删除
					flag2 = true
				if flag2
					根据cost对vector<bunch_id, cost>进行排序，从小到大
			*/
			for (std::vector<std::pair<unsigned int, std::vector<std::pair<unsigned int, int>>>>::iterator
				currIt(addedDealyTable.begin()); currIt != addedDealyTable.end();)
			{
				std::shared_ptr<FlightInfo> pThisInfo(infoMap.find(currIt->first)->second);

				std::vector<std::pair<unsigned int, int>>::iterator pSelectBunch(
					std::find_if(currIt->second.begin(), currIt->second.end(), [bunchId]
					(std::pair<unsigned int, int> &lop) -> bool
					{
						return lop.first == bunchId;
					})
				);
				if (pSelectBunch == currIt->second.end())
				{
					++currIt;
				}
				else
				{
					Time newCost(pNewPlan->m_bunches[bunchId].addedDelayIfAddFlight(pThisInfo));
					if (newCost != SpecialTime::MaxTime)
					{
						pSelectBunch->second = newCost.totalMins();

						std::sort(currIt->second.begin(), currIt->second.end(),
							[](std::pair<unsigned int, int> &lop,
								std::pair<unsigned int, int> &rop) -> bool
						{
							return lop.second < rop.second;
						});

						++currIt;
					}
					else
					{
						currIt->second.erase(pSelectBunch);

						if (!currIt->second.empty())
						{
							++currIt;
						}
						else
						{
							unsigned int p(0), q(pNewPlan->m_bunches.size());
							for (; p != q && pNewPlan->m_bunches[p].size() != 0; ++p);
							if (p != q)
							{
								pNewPlan->m_bunches[p].addFlight(infoMap.find(currIt->first)->second);
								tCopy[currIt->first] = p;
								currIt = addedDealyTable.erase(currIt);
							}
							else
							{
								flag = true;
								break;
							}
						}
					}
				}

				if (flag)
					break;
			}
			if (flag)
				break;
		}
	}

	if (!flag)
	{
		for (const FlightBunch &bunch : pNewPlan->m_bunches)
			pNewPlan->totalDelay += bunch.delay();
		t = std::move(tCopy);

		return pNewPlan;
	}
	else
	{
		return nullptr;
	}
}

PlanTable FlightPlan::getPlanTable(void) const
{
	PlanTable ret(orgPlanTable);

	for (unsigned int i(0), j(m_bunches.size()); i != j; ++i)
	{
		for (unsigned int m(0), n(m_bunches[i].size()); m != n; ++m)
		{
			ret[m_bunches[i][m].info().id] = i;
		}
	}

	return std::move(ret);
}

const std::vector<FlightBunch> &FlightPlan::bunches(void) const
{
	return m_bunches;
}

const Time &FlightPlan::delay(void) const
{
	return totalDelay;
}

FlightPlan::FlightPlan()
	: m_bunches(orgBunches), totalDelay(Time(0, 0))
{

}
