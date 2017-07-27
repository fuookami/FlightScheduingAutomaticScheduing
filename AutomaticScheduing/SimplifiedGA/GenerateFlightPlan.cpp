#include "GenerateFlightPlan.h"

#include <fstream>
#include <thread>
#include <algorithm>
#include <unordered_map>

namespace GenerateFlightPlan
{
	unsigned int FlighterNum = 0;
	unsigned int FlightInfoNum = 0;

	FlightInfoSet flightInfoSet;
	FlightInfoMap flightInfoMap;

	void loadDatas(const std::string &dataInputFileName)
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

	void run(bool FaultToTerant, SolveFunction_t solveFun, const std::string &dataOutputFileName)
	{
		std::vector<PlanTable> initialSolution(SubFun::generateInitialSolution());
		OutputDatas outputData(solveFun(initialSolution, FaultToTerant, std::make_pair(0, flightInfoSet.size()), 
			&SubFun::planTable2Score, &SubFun::ComparePlanTable));
		SubFun::outputDatas(outputData, dataOutputFileName);
	}

	void testInitialSolution(const unsigned int time)
	{
		std::ofstream fout("ResultTestInitialSolution.txt");
		std::unordered_map<unsigned int, unsigned int> counter;

		unsigned int num(0);
		for (unsigned int i(0), j(time / GenerateFlightPlan::FlighterNum + 1); i != j; ++i)
		{
			std::vector<PlanTableWithScore> thisInitialSolution(SubFun::planTable2Score(SubFun::generateInitialSolution()));
			for (const PlanTableWithScore &thisSolution : thisInitialSolution)
			{
				++counter[thisSolution.second];
			}

			num += GenerateFlightPlan::FlightInfoNum;
			std::cout << num << std::endl;
		}

		for (const std::pair<unsigned int, unsigned int> &pair : counter)
		{
			fout << pair.first << ' ' << pair.second << std::endl;
		}
		fout.close();
		return;
	}

	namespace SubFun
	{
		std::vector<PlanTable> generateInitialSolution(void)
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

		std::vector<std::pair<PlanTable, unsigned int>> planTable2Score(const std::vector<PlanTable> &planTables, bool FaultTolerant)
		{
			std::vector<std::pair<PlanTable, unsigned int>> ret;
			std::vector<std::thread> threads;
			static auto calScore = [FaultTolerant](std::pair<PlanTable, unsigned int> *pPair) {
				std::shared_ptr<FlightPlan> flightPlan(FaultTolerant ? FlightPlan::generateFromPlanTableWithFaultTolerant(pPair->first, flightInfoMap) :
					FlightPlan::generateFromPlanTable(pPair->first, flightInfoMap));
				if (flightPlan != nullptr)
					pPair->second = flightPlan->delay().totalMins();
				else
					pPair->second = UINT_MAX;
			};

			for (unsigned int i(0), j(planTables.size()); i != j; ++i)
				ret.emplace_back(std::make_pair(planTables[i], 0));

			for (unsigned int i(0), j(planTables.size()); i != j; ++i)
				threads.push_back(std::thread(calScore, &ret[i]));

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

		bool ComparePlanTable(const std::pair<PlanTable, unsigned int> & lop, const std::pair<PlanTable, unsigned int> & rop)
		{	
			return lop.second > rop.second;
		}

		void outputDatas(const OutputDatas &datas, const std::string & dataOutputFileName)
		{
			std::ofstream fout(dataOuputFilter + dataOutputFileName);
			
			unsigned int size(datas.mutationRateOfIters.size());
			fout << size << std::endl;
			for (unsigned int i(0), j(size); i != j; ++i)
			{
				fout << datas.minAndMaxPopulationQuantityOfIters[i].first << ' '
					<< datas.minAndMaxPopulationQuantityOfIters[i].second << ' '
					<< datas.minAndMaxScoresOfIters[i].first << ' '
					<< datas.minAndMaxScoresOfIters[i].second << ' '
					<< datas.mutationRateOfIters[i] << std::endl;
			}
			fout.close();

			fout.open(dataOuputFilter + "BestPlanTable.txt");

			std::shared_ptr<FlightPlan> plan(FlightPlan::generateFromPlanTable(datas.bestPair.first, flightInfoMap));
			unsigned int i = 1;
			Time total_delay(0, 0);
			for (const FlightBunch &bunch : plan->bunches())
			{
				fout << "Aircraft" << i << ": " << std::endl;
				for (const Flight &flight : bunch.flights())
				{
					fout << flight << std::endl;
				}
				fout << std::endl;
				total_delay += bunch.delay();
				++i;
			}
			fout << "Total delay: " << plan->delay().totalMins() << std::endl;

			fout.close();
		}
	};
};