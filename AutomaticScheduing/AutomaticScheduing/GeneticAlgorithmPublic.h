#pragma once

#include <vector>
#include <map>
#include <string>
#include <utility>

namespace UICodeGeneticAlgorithm
{
	using UICodeSolt = std::vector<unsigned int>;
	using UICodeSoltFitnessPair = std::pair<UICodeSolt, unsigned int>;
	using UIntSolt2SoltFitenessPair = std::vector<UICodeSoltFitnessPair>(const std::vector<UICodeSolt> &);

	bool BiggerIsBetter(const UICodeSoltFitnessPair &thisPair, const UICodeSoltFitnessPair &best);
	bool SmallerIsBetter(const UICodeSoltFitnessPair &thisPair, const UICodeSoltFitnessPair &best);
	using CompareFun = bool(const UICodeSoltFitnessPair &thisPair, const UICodeSoltFitnessPair &best);

	class SettingArg
	{
		friend bool operator==(const SettingArg &lop, const unsigned int rop);
	public:
		const std::string &code() const { return _map.find(_code)->second; }
		const unsigned int toUInt() const { return _code; }

	protected:
		SettingArg(std::map<unsigned int, std::string> map, unsigned int c)
			: _map(map), _code(c) {}

	private:
		std::map<unsigned int, std::string> _map;
		unsigned int _code;
	};

	class FitnessType : public SettingArg
	{
	public:
		enum
		{
			BiggerBetter,
			SmallerBetter
		};

		static const std::map<unsigned int, std::string> names;

		explicit FitnessType(unsigned int c) :
			SettingArg(names, c) {};
	};

	class PopulationNum : public SettingArg
	{
	public:
		enum
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

	class PopulationComunicationMode : public SettingArg
	{
	public:
		enum
		{
			Random,
			BetterToWrose,
			MoreToLess,
		};

		static const std::map<unsigned int, std::string> names;

		explicit PopulationComunicationMode(unsigned int c) :
			SettingArg(names, c) {};
	};

	class SelectMode : public SettingArg
	{
	public:
		enum
		{
			Static,
			AdapativeDynamic,
		};

		static const std::map<unsigned int, std::string> names;

		explicit SelectMode(unsigned int c) :
			SettingArg(names, c) {};
	};

	class SelectOperator : public SettingArg
	{
	public:
		enum
		{
			Roulette,
			//BestIndividual,
			RankSelect,
			Tournament
		};

		static const std::map<unsigned int, std::string> names;

		explicit SelectOperator(unsigned int c) :
			SettingArg(names, c) {};
	};

	class CrossMode : public SettingArg
	{
	public:
		enum
		{
			OneParent,
			TwoParent,
			MultiParent,
			AdapativeMultiParent,
		};

		static const std::map<unsigned int, std::string> names;

		explicit CrossMode(const unsigned int i) :
			SettingArg(names, i) {}
	};

	class CrossOperator : public SettingArg
	{
	public:
		enum
		{
			OnePoint,
			TwoPoint,
			MultiPoint,
			Uniform,
			Cycle,
			//Shuffle,
		};

		static const std::map<unsigned int, std::string> names;

		explicit CrossOperator(unsigned int c) :
			SettingArg(names, c) {};
	};

	class MutationRateMode : public SettingArg
	{
	public:
		enum
		{
			Static,
			AdapativeDynamic
		};

		static const std::map<unsigned int, std::string> names;

		explicit MutationRateMode(unsigned int c) :
			SettingArg(names, c) {};
	};

	class MutationOperator : public SettingArg
	{
	public:
		enum
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

	struct SettingHelper
	{
		SettingHelper(unsigned int _range, unsigned int _length, std::pair<unsigned int, unsigned int> _individualNumber,
			FitnessType _fitnessType, PopulationNum _populationNum, PopulationComunicationMode _populationComunicationMode, 
			SelectMode _selectMode, SelectOperator _selectOperation, CrossMode _crossMode, CrossOperator _crossOperator, 
			MutationRateMode _mutationRateMode, MutationOperator _mutationOperator) :
			range(_range), length(_length), individualNumber(_individualNumber), fitnessType(_fitnessType), 
			populationNum(_populationNum), populationComunicationMode(_populationComunicationMode), 
			selectMode(_selectMode), selectOperator(_selectOperation), crossMode(_crossMode), 
			crossOperator(_crossOperator), mutationRateMode(_mutationRateMode), mutationOperator(_mutationOperator) {}

		std::string toString() const;

		unsigned int range;
		unsigned int length;

		std::pair<unsigned int, unsigned int> individualNumber;

		FitnessType fitnessType;

		PopulationNum populationNum;
		PopulationComunicationMode populationComunicationMode;

		SelectMode selectMode;
		SelectOperator selectOperator;

		CrossMode crossMode;
		CrossOperator crossOperator;

		MutationRateMode mutationRateMode;
		MutationOperator mutationOperator;
	};
}