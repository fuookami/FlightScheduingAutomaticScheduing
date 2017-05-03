#include "GeneticAlgorithmPublic.h"

#include <iostream>
#include <sstream>

namespace UICodeGeneticAlgorithm
{
	std::string SettingHelper::toString() const
	{
		std::ostringstream sout;
		sout << "基因取值范围： 0 ~ " << this->range << std::endl;
		sout << "染色体长度：" << this->length << std::endl;
		sout << "种群密度范围： " << individualNumber.first << " ~ " << individualNumber.second << std::endl;
		sout << "种群数目：" << this->populationNum.code() << std::endl;
		sout << "种群交流模式：" << this->populationComunicationMode.code() << std::endl;
		sout << "种群密度模式：" << this->selectMode.code() << std::endl;
		sout << "选择算子：" << this->selectOperator.code() << std::endl;
		sout << "交叉模式：" << this->crossMode.code() << std::endl;
		sout << "交叉算子：" << this->crossOperator.code() << std::endl;
		sout << "变异概率模式：" << this->mutationRateMode.code() << std::endl;
		sout << "变异算子" << this->mutationOperator.code() << std::endl;

		return std::move(sout.str());
	}
}