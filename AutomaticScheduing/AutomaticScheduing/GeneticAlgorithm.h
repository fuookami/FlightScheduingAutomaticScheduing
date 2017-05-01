#pragma once

#include <vector>
#include <map>
#include <utility>

namespace UIntegerCodeGeneticAlgorithm
{
	using UIntegerCodeSolution = std::vector<unsigned int>;
	using UIntegerCodeSolutionFitnessPair = std::pair<UIntegerCodeSolution, unsigned int>;
	using UIntSolt2SoltFitenessPair = std::vector<UIntegerCodeSolutionFitnessPair> (const std::vector<UIntegerCodeSolution> &);

	class SettingSubArgument
	{
	public:
		const std::string &code() const { return _map.find(_code)->second; }

	protected:
		SettingSubArgument(std::map<unsigned int, std::string> map, unsigned int c)
			: _map(map), _code(c) {}

	private:
		std::map<unsigned int, std::string> _map;
		unsigned int _code;
	};

	class SelectMode : public SettingSubArgument
	{
	public:
		static enum
		{
			Static,
			AdapativeDynamic,
		};

		static const std::map<unsigned int, std::string> names;

		SelectMode(unsigned int c) :
			SettingSubArgument(names, c) {};
	};

	const std::map<unsigned int, std::string> SelectMode::names = {
		std::make_pair(SelectMode::Static,			"��̬��Ⱥ����"),
		std::make_pair(SelectMode::AdapativeDynamic,"����Ӧ��̬��Ⱥ����"),
	};

	class SelectOperator : public SettingSubArgument
	{
	public:
		static enum
		{
			Roulette,
			BestIndividual,
			RankSelect,
			Tournament
		};

		static const std::map<unsigned int, std::string> names;

		SelectOperator(unsigned int c) :
			SettingSubArgument(names, c) {};
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(SelectOperator::Roulette,		"���̶�ѡ��"),
		std::make_pair(SelectOperator::BestIndividual,	"��Ѹ���ѡ��"),
		std::make_pair(SelectOperator::RankSelect,		"����ѡ��"),
		std::make_pair(SelectOperator::Tournament,		"������ѡ��")
	};

	class CrossMode : public SettingSubArgument
	{
	public:
		static enum 
		{
			TwoParent,
			MultiParent,
			AdapativeMultiParent,
		};

		static const std::map<unsigned int, std::string> names;
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(CrossMode::TwoParent,			"˫��������"),
		std::make_pair(CrossMode::MultiParent,			"�ุ������"),
		std::make_pair(CrossMode::AdapativeMultiParent,	"����Ӧ�ุ������"),
	};

	class CrossOperator : public SettingSubArgument
	{
	public:
		static enum
		{
			OnePoint,
			TwoPoint,
			MultiPoint,
			Uniform,
			Shuffle,
		};

		static const std::map<unsigned int, std::string> names;

		CrossOperator(unsigned int c) :
			SettingSubArgument(names, c) {};
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(CrossOperator::OnePoint,		"���㽻������"),
		std::make_pair(CrossOperator::TwoPoint,		"���㽻������"),
		std::make_pair(CrossOperator::MultiPoint,	"��㽻������"),
		std::make_pair(CrossOperator::Uniform,		"���Ƚ�������"),
		std::make_pair(CrossOperator::Shuffle,		"ϴ�ƽ�������")
	};

	class MutationOperator : public SettingSubArgument
	{
	public:
		static enum
		{
			Uniform,
			NonUniform,
			Boundary,
			Gaussian,
		};

		static const std::map<unsigned int, std::string> names;

		MutationOperator(unsigned int c) :
			SettingSubArgument(names, c) {};
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(CrossOperator::OnePoint,		"���ȱ�������"),
		std::make_pair(CrossOperator::TwoPoint,		"�Ǿ��ȱ�������"),
		std::make_pair(CrossOperator::MultiPoint,	"�߽��������"),
		std::make_pair(CrossOperator::Uniform,		"��˹��������")
	};

	struct SettingHelper
	{
		unsigned int range;
	};

	UIntegerCodeSolution run(std::vector<UIntegerCodeSolution> initialSolution,
		SettingHelper setting, UIntSolt2SoltFitenessPair *solt2ScoreTransFun);
};