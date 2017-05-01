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
		std::make_pair(SelectMode::Static,			"静态种群个数"),
		std::make_pair(SelectMode::AdapativeDynamic,"自适应动态种群个数"),
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
		std::make_pair(SelectOperator::Roulette,		"轮盘赌选择法"),
		std::make_pair(SelectOperator::BestIndividual,	"最佳个体选择法"),
		std::make_pair(SelectOperator::RankSelect,		"排序选择法"),
		std::make_pair(SelectOperator::Tournament,		"竞标赛选择法")
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
		std::make_pair(CrossMode::TwoParent,			"双父辈交叉"),
		std::make_pair(CrossMode::MultiParent,			"多父辈交叉"),
		std::make_pair(CrossMode::AdapativeMultiParent,	"自适应多父辈交叉"),
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
		std::make_pair(CrossOperator::OnePoint,		"单点交叉算子"),
		std::make_pair(CrossOperator::TwoPoint,		"两点交叉算子"),
		std::make_pair(CrossOperator::MultiPoint,	"多点交叉算子"),
		std::make_pair(CrossOperator::Uniform,		"均匀交叉算子"),
		std::make_pair(CrossOperator::Shuffle,		"洗牌交叉算子")
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
		std::make_pair(CrossOperator::OnePoint,		"均匀变异算子"),
		std::make_pair(CrossOperator::TwoPoint,		"非均匀变异算子"),
		std::make_pair(CrossOperator::MultiPoint,	"边界变异算子"),
		std::make_pair(CrossOperator::Uniform,		"高斯变异算子")
	};

	struct SettingHelper
	{
		unsigned int range;
	};

	UIntegerCodeSolution run(std::vector<UIntegerCodeSolution> initialSolution,
		SettingHelper setting, UIntSolt2SoltFitenessPair *solt2ScoreTransFun);
};