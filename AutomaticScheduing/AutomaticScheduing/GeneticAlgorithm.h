#pragma once

#include <vector>
#include <map>
#include <utility>

namespace UICodeGeneticAlgorithm
{
	using UICodeSolt = std::vector<unsigned int>;
	using UICodeSoltFitnessPair = std::pair<UICodeSolt, unsigned int>;
	using UIntSolt2SoltFitenessPair = std::vector<UICodeSoltFitnessPair> (const std::vector<UICodeSolt> &);

	static const unsigned int MaxGeneration = 10;

	class SettingArg
	{
	public:
		const std::string &code() const { return _map.find(_code)->second; }

	protected:
		SettingArg(std::map<unsigned int, std::string> map, unsigned int c)
			: _map(map), _code(c) {}

	private:
		std::map<unsigned int, std::string> _map;
		unsigned int _code;
	};

	class PopulationNum : public SettingArg
	{
	public:
		static enum
		{
			One,
			Two,
			Three,
			Four,
			Five,
			Six,
			Seven,
			Eight
		};

		static const std::map<unsigned int, std::string> names;

		PopulationNum(unsigned int c) :
			SettingArg(names, c) {};
	};

	const std::map<unsigned int, std::string> PopulationNum::names = {
		std::make_pair(One,		"单种群"),
		std::make_pair(Two,		"双种群"),
		std::make_pair(Three,	"三种群"),
		std::make_pair(Four,	"四种群"),
		std::make_pair(Five,	"五种群"),
		std::make_pair(Six,		"六种群"),
		std::make_pair(Seven,	"七种群"),
		std::make_pair(Eight,	"八种群")
	};

	class PopulationComunicationMode : public SettingArg
	{
	public:
		static enum
		{
			Random,
			BetterToWrose,
			MoreToLess,
		};

		static const std::map<unsigned int, std::string> names;

		PopulationComunicationMode(unsigned int c) :
			SettingArg(names, c) {};
	};

	const std::map<unsigned int, std::string> PopulationComunicationMode::names = {
		std::make_pair(Random,			"种群个体随机交流"),
		std::make_pair(BetterToWrose,	"优秀种群的个体向较差种群流动"),
		std::make_pair(MoreToLess,		"较大种群密度的个体向较小种群密度的个体流动")
	};

	class SelectMode : public SettingArg
	{
	public:
		static enum
		{
			Static,
			AdapativeDynamic,
		};

		static const std::map<unsigned int, std::string> names;

		SelectMode(unsigned int c) :
			SettingArg(names, c) {};
	};

	const std::map<unsigned int, std::string> SelectMode::names = {
		std::make_pair(SelectMode::Static,			"静态种群个数"),
		std::make_pair(SelectMode::AdapativeDynamic,"自适应动态种群密度"),
	};

	class SelectOperator : public SettingArg
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
			SettingArg(names, c) {};
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(SelectOperator::Roulette,		"轮盘赌选择法"),
		std::make_pair(SelectOperator::BestIndividual,	"最佳个体选择法"),
		std::make_pair(SelectOperator::RankSelect,		"排序选择法"),
		std::make_pair(SelectOperator::Tournament,		"竞标赛选择法")
	};

	class CrossMode : public SettingArg
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

	const std::map<unsigned int, std::string> CrossMode::names = {
		std::make_pair(CrossMode::TwoParent,			"双父辈交叉"),
		std::make_pair(CrossMode::MultiParent,			"多父辈交叉"),
		std::make_pair(CrossMode::AdapativeMultiParent,	"自适应多父辈交叉"),
	};

	class CrossOperator : public SettingArg
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
			SettingArg(names, c) {};
	};

	const std::map<unsigned int, std::string> CrossOperator::names = {
		std::make_pair(CrossOperator::OnePoint,		"单点交叉算子"),
		std::make_pair(CrossOperator::TwoPoint,		"两点交叉算子"),
		std::make_pair(CrossOperator::MultiPoint,	"多点交叉算子"),
		std::make_pair(CrossOperator::Uniform,		"均匀交叉算子"),
		std::make_pair(CrossOperator::Shuffle,		"洗牌交叉算子")
	};

	class MutationRateMode : public SettingArg
	{
	public:
		static enum
		{
			Static,
			AdapativeDynamic
		};

		static const std::map<unsigned int, std::string> names;

		MutationRateMode(unsigned int c) :
			SettingArg(names, c) {};
	};

	const std::map<unsigned int, std::string> MutationRateMode::names = {
		std::make_pair(Static,			"静态变异概率"),
		std::make_pair(AdapativeDynamic,"自适应动态变异概率")
	};

	class MutationOperator : public SettingArg
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
			SettingArg(names, c) {};
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
		unsigned int length;

		PopulationNum populationNum;
		PopulationComunicationMode populationComunicationMode;

		SelectMode selectMode;
		SelectOperator selectOperator;

		CrossMode crossMode;
		CrossOperator crossOperator;

		MutationRateMode mutationRateMode;
		MutationOperator mutationOperator;
	};

	UICodeSolt run(std::vector<UICodeSolt> initialSolution,
		SettingHelper setting, UIntSolt2SoltFitenessPair *solt2ScoreTransFun);

	namespace Population
	{
		using UICodeSoltPopulation = struct
		{
			std::vector<UICodeSoltFitnessPair> pairs;
			UICodeSoltFitnessPair best;
		};

		std::vector<UICodeSoltPopulation> run(const std::vector<UICodeSoltPopulation> &populations, const SettingHelper &setting);
	}

	namespace Select
	{
		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};

	namespace Cross
	{
		std::vector<UICodeSolt> run(const std::vector<UICodeSolt> &solts, const SettingHelper &setting);
	};

	namespace Mutation
	{
		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};
};