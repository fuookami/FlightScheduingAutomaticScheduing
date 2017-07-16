#include "GenerateFlightPlan.h"
#include "GAFrame.h"

int main(void)
{
	GenerateFlightPlan::loadDatas("test2.txt");
	GenerateFlightPlan::run(true, &GA::run, "result2.txt");

	system("pause");
	return 0;
}