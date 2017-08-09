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
					addedDelays.push_back(std::make_pair(i, thisDelay.totalMins()));
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
				unsigned int i(0), j(pNewPlan->bunches.size());
				for (; i != j && pNewPlan->bunches[i].size() != 0; ++i);
				if (i == j)
				{
					flag = true;
					break;
				}
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
	PlanTable tCopy;
	while (flag)
	{
		pNewPlan.reset(new FlightPlan());
		tCopy = t;
		flag = false;

		std::vector<std::pair<unsigned int, std::vector<std::pair<unsigned int, int>>>> addedDealyTable;
		for (unsigned int i(0), j(tCopy.size()); i != j; ++i)
		{
			std::shared_ptr<FlightInfo> pThisFlight(infoMap.find(i)->second);

			if (!pNewPlan->bunches[tCopy[i]].addFlight(pThisFlight))
			{
				if (d(gen) < maxRank)
					addedDealyTable.push_back(std::make_pair(i, 
						std::vector<std::pair<unsigned int, int>>()));
				else
				{
					// Ѱ��һ���պ��മ
					unsigned int p(0), q(pNewPlan->bunches.size());
					for (; p != q && pNewPlan->bunches[p].size() != 0; ++p);
					if (p != q)
					{
						// ���뵽�������മ��
						pNewPlan->bunches[p].addFlight(pThisFlight);
						tCopy[i] = p;
					}
					else
					{
						// ����addedDealyTable����
						addedDealyTable.push_back(std::make_pair(i, 
							std::vector<std::pair<unsigned int, int>>()));
					}
				}
			}
		}

		/*
		for <flight_id, vector<bunch_id, cost>> in addedDealyTable
			for bunch in pNewPlan
				if bunch.cost(flight)���������
					��<bunch_id, cost>����addedDealyTable[flight_id]
			if ��������к��മ������ȥ
				����cost��vector<bunch_id, cost>�������򣬴�С����
			else
				Ѱ��һ���պ��മ
				if �����º���
					���뵽����������
				else
					��������
		*/
		for (std::pair<unsigned int, std::vector<std::pair<unsigned int, int>>> 
			&ele : addedDealyTable)
		{
			std::shared_ptr<FlightInfo> pThisFlight(infoMap.find(ele.first)->second);

			for (unsigned int i(0), j(pNewPlan->bunches.size()); i != j; ++i)
			{
				Time cost(pNewPlan->bunches[i].addedDelayIfAddFlight(pThisFlight));
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
				unsigned int p(0), q(pNewPlan->bunches.size());
				for (; p != q && pNewPlan->bunches[p].size() != 0; ++p);
				if (p != q)
				{
					pNewPlan->bunches[p].addFlight(pThisFlight);
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
			��������������ĺ���id��ʾδ���뺽�മ����
			*/

			/*
			���� vector<bunch_id, cost>[0].cost �� addedDealyTable(<flight_id, vector<bunch_id, cost>>) �������򣬴�С����
			���ɷֲ�ѡ��һ�������������addedDealyTable
			���ɷֲ�ѡ��bunch_id���߼��뵽�µĺ��മ��
			�Ƿ���ĺ��മΪnew_bunch_id
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
			pNewPlan->bunches[bunchId].addFlight(infoMap.find(pSelect->first)->second);
			addedDealyTable.erase(pSelect);

			/*
			����addedDealyTable��
			for <flight_id, vector<bunch_id, cost>> in addedDealyTable
				bool flag2
				if new_bunch_id ������ vector<bunch_id, cost> ��
					if bunch.cost(flight) ���������
						����cost
					else
						ɾ��
					flag2 = true
				if flag2
					����cost��vector<bunch_id, cost>�������򣬴�С����
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
				if (pSelectBunch != currIt->second.end())
				{
					Time newCost(pNewPlan->bunches[bunchId].addedDelayIfAddFlight(pThisInfo));
					if (newCost != SpecialTime::MaxTime)
					{
						pSelectBunch->second = newCost.totalMins();
						++currIt;

						std::sort(currIt->second.begin(), currIt->second.end(),
							[](std::pair<unsigned int, int> &lop,
								std::pair<unsigned int, int> &rop) -> bool
						{
							return lop.second < rop.second;
						});
					}
					else
					{
						currIt->second.erase(pSelectBunch);

						if (currIt->second.empty())
						{
							unsigned int p(0), q(pNewPlan->bunches.size());
							for (; p != q && pNewPlan->bunches[p].size() != 0; ++p);
							if (p != q)
							{
								pNewPlan->bunches[p].addFlight(infoMap.find(currIt->first)->second);
								tCopy[currIt->first] = p;
								currIt = addedDealyTable.erase(currIt);
							}
							else
							{
								flag = true;
								break;
							}
						}
						else
							++currIt;
					}
				}

				if (flag)
					break;
			}

			if (flag)
				break;
		}
	}

	for (const FlightBunch &bunch : pNewPlan->bunches)
		pNewPlan->totalDelay += bunch.delay();
	
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
