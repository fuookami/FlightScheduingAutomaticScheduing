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
		sout << "基因取值范围： 0 ~ " << this->range << std::endl;
		sout << "染色体长度： " << this->length << std::endl;
		sout << "适应度模式： " << this->fitnessType.code() << std::endl;
		sout << "种群数目： " << this->populationNum.code() << std::endl;
		sout << "种群交流模式： " << this->populationComunicationMode.code() << std::endl;
		sout << "种群密度模式： " << this->selectMode.code() << std::endl;
		if (this->selectOperator.toUInt() != SelectMode::Static)
			sout << "种群密度范围： " << individualNumber.first << " ~ " << individualNumber.second << std::endl;
		else
			sout << "种群密度： " << individualNumber.first << std::endl;
		sout << "选择算子： " << this->selectOperator.code() << std::endl;
		sout << "交叉模式： " << this->crossMode.code() << std::endl;
		sout << "交叉算子： " << this->crossOperator.code() << std::endl;
		sout << "变异概率模式： " << this->mutationRateMode.code() << std::endl;
		sout << "变异算子： " << this->mutationOperator.code() << std::endl;

		return std::move(sout.str());
	}

	const std::map<unsigned int, std::string> FitnessType::names = {
		std::make_pair(BiggerBetter,	"较大为优"),
		std::make_pair(SmallerBetter,	"较小为优")
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

	const std::map<unsigned int, std::string> PopulationComunicationMode::names = {
		std::make_pair(Random,			"种群个体随机交流"),
		std::make_pair(BetterToWrose,	"优秀种群的个体向较差种群流动"),
		std::make_pair(MoreToLess,		"较大种群密度的个体向较小种群密度的个体流动")
	};

	const std::map<unsigned int, std::string> SelectMode::names = {
		std::make_pair(SelectMode::Static,			"静态种群个数"),
		std::make_pair(SelectMode::AdapativeDynamic,"自适应动态种群密度"),
	};

	const std::map<unsigned int, std::string> SelectOperator::names = {
		std::make_pair(SelectOperator::Roulette,		"轮盘赌选择法"),
		//std::make_pair(SelectOperator::BestIndividual,	"最佳个体选择法"),
		std::make_pair(SelectOperator::RankSelect,		"排序选择法"),
		std::make_pair(SelectOperator::Tournament,		"锦标赛选择法")
	};

	const std::map<unsigned int, std::string> CrossMode::names = {
		std::make_pair(CrossMode::OneParent,			"单父辈交叉"),
		std::make_pair(CrossMode::TwoParent,			"双父辈交叉"),
		std::make_pair(CrossMode::MultiParent,			"多父辈交叉"),
		std::make_pair(CrossMode::AdapativeMultiParent,	"自适应多父辈交叉"),
	};

	const std::map<unsigned int, std::string> CrossOperator::names = {
		std::make_pair(CrossOperator::OnePoint,		"单点交叉算子"),
		std::make_pair(CrossOperator::TwoPoint,		"两点交叉算子"),
		std::make_pair(CrossOperator::MultiPoint,	"多点交叉算子"),
		std::make_pair(CrossOperator::Uniform,		"均匀交叉算子"),
		std::make_pair(CrossOperator::Cycle,		"环交叉算子"),
		//std::make_pair(CrossOperator::Shuffle,		"洗牌交叉算子")
	};

	const std::map<unsigned int, std::string> MutationRateMode::names = {
		std::make_pair(Static,			"静态变异概率"),
		std::make_pair(AdapativeDynamic,"自适应动态变异概率")
	};

	const std::map<unsigned int, std::string> MutationOperator::names = {
		std::make_pair(MutationOperator::Uniform,		"均匀变异算子"),
		std::make_pair(MutationOperator::NonUniform,	"非均匀变异算子"),
		//std::make_pair(MutationOperator::Boundary,		"边界变异算子"),
		std::make_pair(MutationOperator::Gaussian,		"高斯变异算子")
	};
}