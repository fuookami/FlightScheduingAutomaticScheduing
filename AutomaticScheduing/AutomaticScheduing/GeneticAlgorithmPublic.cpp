#include "GeneticAlgorithmPublic.h"

#include <iostream>
#include <sstream>

namespace UICodeGeneticAlgorithm
{
	std::string SettingHelper::toString() const
	{
		std::ostringstream sout;
		sout << "����ȡֵ��Χ�� 0 ~ " << this->range << std::endl;
		sout << "Ⱦɫ�峤�ȣ�" << this->length << std::endl;
		sout << "��Ⱥ�ܶȷ�Χ�� " << individualNumber.first << " ~ " << individualNumber.second << std::endl;
		sout << "��Ⱥ��Ŀ��" << this->populationNum.code() << std::endl;
		sout << "��Ⱥ����ģʽ��" << this->populationComunicationMode.code() << std::endl;
		sout << "��Ⱥ�ܶ�ģʽ��" << this->selectMode.code() << std::endl;
		sout << "ѡ�����ӣ�" << this->selectOperator.code() << std::endl;
		sout << "����ģʽ��" << this->crossMode.code() << std::endl;
		sout << "�������ӣ�" << this->crossOperator.code() << std::endl;
		sout << "�������ģʽ��" << this->mutationRateMode.code() << std::endl;
		sout << "��������" << this->mutationOperator.code() << std::endl;

		return std::move(sout.str());
	}
}