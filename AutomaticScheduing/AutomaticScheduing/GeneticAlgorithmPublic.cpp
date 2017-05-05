#include "GeneticAlgorithmPublic.h"

#include <iostream>
#include <sstream>

namespace UICodeGeneticAlgorithm
{
	bool BiggerIsBetter(const UICodeSoltFitnessPair &thisPair, const UICodeSoltFitnessPair &best)
	{
		return thisPair.second > best.second;
	}
	bool SmallerIsBetter(const UICodeSoltFitnessPair &thisPair, const UICodeSoltFitnessPair &best)
	{
		return thisPair.second < best.second;
	}

	bool operator==(const SettingArg &lop, const unsigned int rop) 
	{ 
		return lop._code == rop; 
	}

	std::string SettingHelper::toString() const
	{
		std::ostringstream sout;
		sout << "����ȡֵ��Χ�� 0 ~ " << this->range << std::endl;
		sout << "Ⱦɫ�峤�ȣ� " << this->length << std::endl;
		sout << "��Ӧ��ģʽ�� " << this->fitnessType.code() << std::endl;
		sout << "��Ⱥ��Ŀ�� " << this->populationNum.code() << std::endl;
		sout << "��Ⱥ����ģʽ�� " << this->populationComunicationMode.code() << std::endl;
		sout << "��Ⱥ�ܶ�ģʽ�� " << this->selectMode.code() << std::endl;
		if (this->selectOperator.toUInt() != SelectMode::Static)
			sout << "��Ⱥ�ܶȷ�Χ�� " << individualNumber.first << " ~ " << individualNumber.second << std::endl;
		else
			sout << "��Ⱥ�ܶȣ� " << individualNumber.first << std::endl;
		sout << "ѡ�����ӣ� " << this->selectOperator.code() << std::endl;
		sout << "����ģʽ�� " << this->crossMode.code() << std::endl;
		sout << "�������ӣ� " << this->crossOperator.code() << std::endl;
		sout << "�������ģʽ�� " << this->mutationRateMode.code() << std::endl;
		sout << "�������ӣ� " << this->mutationOperator.code() << std::endl;

		return std::move(sout.str());
	}

	const std::map<unsigned int, std::string> FitnessType::names = {
		std::make_pair(BiggerBetter,	"�ϴ�Ϊ��"),
		std::make_pair(SmallerBetter,	"��СΪ��")
	};

	const std::map<unsigned int, std::string> PopulationNum::names = {
		std::make_pair(One,		"����Ⱥ"),
		std::make_pair(Two,		"˫��Ⱥ"),
		std::make_pair(Three,	"����Ⱥ"),
		std::make_pair(Four,	"����Ⱥ"),
		std::make_pair(Five,	"����Ⱥ"),
		std::make_pair(Six,		"����Ⱥ"),
		std::make_pair(Seven,	"����Ⱥ"),
		std::make_pair(Eight,	"����Ⱥ")
	};

	const std::map<unsigned int, std::string> PopulationComunicationMode::names = {
		std::make_pair(Random,			"��Ⱥ�����������"),
		std::make_pair(BetterToWrose,	"������Ⱥ�ĸ�����ϲ���Ⱥ����"),
		std::make_pair(MoreToLess,		"�ϴ���Ⱥ�ܶȵĸ������С��Ⱥ�ܶȵĸ�������")
	};

	const std::map<unsigned int, std::string> SelectMode::names = {
		std::make_pair(SelectMode::Static,			"��̬��Ⱥ����"),
		std::make_pair(SelectMode::AdapativeDynamic,"����Ӧ��̬��Ⱥ�ܶ�"),
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(SelectOperator::Roulette,		"���̶�ѡ��"),
		//std::make_pair(SelectOperator::BestIndividual,	"��Ѹ���ѡ��"),
		std::make_pair(SelectOperator::RankSelect,		"����ѡ��"),
		std::make_pair(SelectOperator::Tournament,		"������ѡ��")
	};

	const std::map<unsigned int, std::string> CrossMode::names = {
		std::make_pair(CrossMode::OneParent,			"����������"),
		std::make_pair(CrossMode::TwoParent,			"˫��������"),
		std::make_pair(CrossMode::MultiParent,			"�ุ������"),
		std::make_pair(CrossMode::AdapativeMultiParent,	"����Ӧ�ุ������"),
	};

	const std::map<unsigned int, std::string> CrossOperator::names = {
		std::make_pair(CrossOperator::OnePoint,		"���㽻������"),
		std::make_pair(CrossOperator::TwoPoint,		"���㽻������"),
		std::make_pair(CrossOperator::MultiPoint,	"��㽻������"),
		std::make_pair(CrossOperator::Uniform,		"���Ƚ�������"),
		std::make_pair(CrossOperator::Cycle,		"����������"),
		//std::make_pair(CrossOperator::Shuffle,		"ϴ�ƽ�������")
	};

	const std::map<unsigned int, std::string> MutationRateMode::names = {
		std::make_pair(Static,			"��̬�������"),
		std::make_pair(AdapativeDynamic,"����Ӧ��̬�������")
	};

	const std::map<unsigned int, std::string> MutationOperator::names = {
		std::make_pair(MutationOperator::Uniform,		"���ȱ�������"),
		std::make_pair(MutationOperator::NonUniform,	"�Ǿ��ȱ�������"),
		//std::make_pair(MutationOperator::Boundary,		"�߽��������"),
		std::make_pair(MutationOperator::Gaussian,		"��˹��������")
	};
}