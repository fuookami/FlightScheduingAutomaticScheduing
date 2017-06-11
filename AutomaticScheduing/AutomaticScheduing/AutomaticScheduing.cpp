#include "GenerateFlightPlan.h"

int main(void)
{
	GenerateFlightPlan::loadDatas();
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
	setting.range = GenerateFlightPlan::FlighterNum;
	setting.length = GenerateFlightPlan::flightInfoSet.size();
	setting.individualNumber = std::make_pair(GenerateFlightPlan::FlighterNum, GenerateFlightPlan::flightInfoSet.size());
	GenerateFlightPlan::run(true, setting);
}