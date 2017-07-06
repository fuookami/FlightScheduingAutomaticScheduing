#include "GenerateFlightPlan.h"
#include <unordered_map>
#include <thread>
#include <iostream>
#include <fstream>

void runMain(void);
void testInitialSolution(void);

int main(void)
{
	GenerateFlightPlan::loadDatas();
	//runMain();
	testInitialSolution();

	system("pause");
	return 0;
}

void runMain(void)
{
	UICodeGeneticAlgorithm::SettingHelper setting(GenerateFlightPlan::FlighterNum, GenerateFlightPlan::flightInfoSet.size(),
		std::make_pair(GenerateFlightPlan::FlighterNum, GenerateFlightPlan::flightInfoSet.size()),
		UICodeGeneticAlgorithm::FitnessType(UICodeGeneticAlgorithm::FitnessType::SmallerBetter),
		UICodeGeneticAlgorithm::PopulationNum(UICodeGeneticAlgorithm::PopulationNum::Four),
		UICodeGeneticAlgorithm::PopulationComunicationMode(UICodeGeneticAlgorithm::PopulationComunicationMode::BetterToWrose),
		UICodeGeneticAlgorithm::SelectMode(UICodeGeneticAlgorithm::SelectMode::AdapativeDynamic),
		UICodeGeneticAlgorithm::SelectOperator(UICodeGeneticAlgorithm::SelectOperator::Tournament),
		UICodeGeneticAlgorithm::CrossMode(UICodeGeneticAlgorithm::CrossMode::AdapativeMultiParent),
		UICodeGeneticAlgorithm::CrossOperator(UICodeGeneticAlgorithm::CrossOperator::MultiPoint),
		UICodeGeneticAlgorithm::MutationRateMode(UICodeGeneticAlgorithm::MutationRateMode::AdapativeDynamic),
		UICodeGeneticAlgorithm::MutationOperator(UICodeGeneticAlgorithm::MutationOperator::Gaussian));
	GenerateFlightPlan::run(true, setting);
}

void testInitialSolution(void)
{
	unsigned int count(0);
	std::unordered_map<unsigned int, unsigned int> hint;
	for (unsigned int i(0), j(100000 / GenerateFlightPlan::FlighterNum); i != j; ++i)
	{
		std::vector<PlanTable> initialSolution(GenerateFlightPlan::FlighterNum, PlanTable());
		std::vector<std::thread> threads;

		for (unsigned int i(0), j(initialSolution.size()); i != j; ++i)
			threads.push_back(std::thread(
				FlightPlan::generatePlanTableWithRandomGreedyAlgorithm, &(initialSolution[i]), GenerateFlightPlan::flightInfoMap));

		for (auto &thread : threads)
			thread.join();

		std::vector<std::pair<PlanTable, unsigned int>> initialSolutionWithScore(GenerateFlightPlan::SubFun::planTable2Score(initialSolution));

		for (const std::pair<PlanTable, unsigned int> &pair : initialSolutionWithScore)
			++hint[pair.second];

		count += GenerateFlightPlan::FlighterNum;
		std::cout << "Num: " << count << std::endl;
	}

	std::ofstream fout("Result\TestInitialSolution.txt");
	for (const std::pair<unsigned int, unsigned int> &pair : hint)
		fout << pair.first << ' ' << pair.second << std::endl;
}