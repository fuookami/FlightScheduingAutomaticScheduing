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

		explicit PopulationNum(unsigned int c) :
			SettingArg(names, c) {};
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

		explicit PopulationComunicationMode(unsigned int c) :
			SettingArg(names, c) {};
	};
	const std::map<unsigned int, std::string> PopulationComunicationMode::names = {
		std::make_pair(Random,			"��Ⱥ�����������"),
		std::make_pair(BetterToWrose,	"������Ⱥ�ĸ�����ϲ���Ⱥ����"),
		std::make_pair(MoreToLess,		"�ϴ���Ⱥ�ܶȵĸ������С��Ⱥ�ܶȵĸ�������")
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

		explicit SelectMode(unsigned int c) :
			SettingArg(names, c) {};
	};
	const std::map<unsigned int, std::string> SelectMode::names = {
		std::make_pair(SelectMode::Static,			"��̬��Ⱥ����"),
		std::make_pair(SelectMode::AdapativeDynamic,"����Ӧ��̬��Ⱥ�ܶ�"),
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

		explicit SelectOperator(unsigned int c) :
			SettingArg(names, c) {};
	};
	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(SelectOperator::Roulette,		"���̶�ѡ��"),
		std::make_pair(SelectOperator::BestIndividual,	"��Ѹ���ѡ��"),
		std::make_pair(SelectOperator::RankSelect,		"����ѡ��"),
		std::make_pair(SelectOperator::Tournament,		"������ѡ��")
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

		explicit CrossMode(const unsigned int i) :
			SettingArg(names, i) {}
	};
	const std::map<unsigned int, std::string> CrossMode::names = {
		std::make_pair(CrossMode::TwoParent,			"˫��������"),
		std::make_pair(CrossMode::MultiParent,			"�ุ������"),
		std::make_pair(CrossMode::AdapativeMultiParent,	"����Ӧ�ุ������"),
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

		explicit CrossOperator(unsigned int c) :
			SettingArg(names, c) {};
	};
	const std::map<unsigned int, std::string> CrossOperator::names = {
		std::make_pair(CrossOperator::OnePoint,		"���㽻������"),
		std::make_pair(CrossOperator::TwoPoint,		"���㽻������"),
		std::make_pair(CrossOperator::MultiPoint,	"��㽻������"),
		std::make_pair(CrossOperator::Uniform,		"���Ƚ�������"),
		std::make_pair(CrossOperator::Shuffle,		"ϴ�ƽ�������")
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

		explicit MutationRateMode(unsigned int c) :
			SettingArg(names, c) {};
	};
	const std::map<unsigned int, std::string> MutationRateMode::names = {
		std::make_pair(Static,			"��̬�������"),
		std::make_pair(AdapativeDynamic,"����Ӧ��̬�������")
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

		explicit MutationOperator(unsigned int c) :
			SettingArg(names, c) {};
	};
	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(MutationOperator::Uniform,		"���ȱ�������"),
		std::make_pair(MutationOperator::NonUniform,	"�Ǿ��ȱ�������"),
		std::make_pair(MutationOperator::Boundary,		"�߽��������"),
		std::make_pair(MutationOperator::Gaussian,		"��˹��������")
	};

	struct SettingHelper
	{
		SettingHelper(unsigned int _range, unsigned int _length, PopulationNum _populationNum,
			PopulationComunicationMode _populationComunicationMode, SelectMode _selectMode,
			SelectOperator _selectOperation, CrossMode _crossMode, CrossOperator _crossOperator,
			MutationRateMode _mutationRateMode, MutationOperator _mutationOperator) :
			range(_range), length(_length), populationNum(_populationNum),
			populationComunicationMode(_populationComunicationMode), selectMode(_selectMode),
			selectOperator(_selectOperation), crossMode(_crossMode), crossOperator(_crossOperator),
			mutationRateMode(_mutationRateMode), mutationOperator(_mutationOperator) {}

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
		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};

	namespace Mutation
	{
		std::vector<UICodeSolt> run(const std::vector<UICodeSoltFitnessPair> &pairs, const SettingHelper &setting);
	};
};