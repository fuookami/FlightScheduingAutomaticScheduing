#include "FlighPlan.h"

#include <sstream>
#include <iomanip>

FlightInfo::FlightInfo(int _flight_id, const std::string & data) : flight_id(_flight_id)
{
	std::istringstream sin(data);
	sin >> name >> dep_airport >> arr_airport >> plan_dep_time >> plan_arr_time >> delay;
}

bool FlightInfo::canBeFollowedBy(const FlightInfo & ano) const
{
	Time evaluateTime(ano.plan_dep_time - this->plan_arr_time);
	return this->arr_airport == ano.dep_airport && evaluateTime >= SpecialTime::MinMCT;
}

std::string FlightInfo::toString(void) const
{
	std::ostringstream sout;
	sout << *this;
	std::string &str(sout.str());
	return std::move(str);
}

bool operator<(const FlightInfo &lop, const FlightInfo &rop)
{
	if (lop.plan_dep_time != rop.plan_dep_time)
		return lop.plan_dep_time < rop.plan_dep_time;
	else if (lop.plan_arr_time != rop.plan_arr_time)
		return lop.plan_arr_time < rop.plan_arr_time;
	else if (lop.dep_airport != rop.dep_airport)
		return lop.dep_airport < rop.dep_airport;
	else if (lop.arr_airport != rop.arr_airport)
		return lop.arr_airport < rop.arr_airport;
	else if (lop.delay != rop.delay)
		return lop.delay < rop.delay;
	else
		return false;
}

std::ostream & operator<<(std::ostream & os, const FlightInfo & flightInfo)
{
	os << flightInfo.flight_id << ' ' << flightInfo.name << ' ' << flightInfo.dep_airport << ' ' << 
		flightInfo.arr_airport << '\t' << flightInfo.plan_dep_time << ' ' << flightInfo.plan_arr_time << '\t' <<
		std::setw(4) << flightInfo.delay.totalMins();
	return os;
}