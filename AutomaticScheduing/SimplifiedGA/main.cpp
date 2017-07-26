#include "GenerateFlightPlan.h"
#include "GAFrame.h"

int main(void)
{
	GenerateFlightPlan::loadDatas("data2.txt");
	//GenerateFlightPlan::run(true, &GA::run, "result2.txt");
	GenerateFlightPlan::testInitialSolution(1000000);

	system("pause");
	return 0;
}