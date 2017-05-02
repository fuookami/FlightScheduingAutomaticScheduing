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
		std::make_pair(SelectMode::Static,			"��̬��Ⱥ����"),
		std::make_pair(SelectMode::AdapativeDynamic,"����Ӧ��̬��Ⱥ����"),
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
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
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

		CrossOperator(unsigned int c) :
			SettingArg(names, c) {};
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(CrossOperator::OnePoint,		"���㽻������"),
		std::make_pair(CrossOperator::TwoPoint,		"���㽻������"),
		std::make_pair(CrossOperator::MultiPoint,	"��㽻������"),
		std::make_pair(CrossOperator::Uniform,		"���Ƚ�������"),
		std::make_pair(CrossOperator::Shuffle,		"ϴ�ƽ�������")
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
		std::make_pair(CrossOperator::OnePoint,		"���ȱ�������"),
		std::make_pair(CrossOperator::TwoPoint,		"�Ǿ��ȱ�������"),
		std::make_pair(CrossOperator::MultiPoint,	"�߽��������"),
		std::make_pair(CrossOperator::Uniform,		"��˹��������")
	};

	struct SettingHelper
	{
		unsigned int range;
		unsigned int length;

		SelectMode selectMode;
		SelectOperator selectOperator;

		CrossMode crossMode;
		CrossOperator crossOperator;

		MutationOperator mutationOperator;
	};

	UICodeSolt run(std::vector<UICodeSolt> initialSolution,
		SettingHelper setting, UIntSolt2SoltFitenessPair *solt2ScoreTransFun);

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