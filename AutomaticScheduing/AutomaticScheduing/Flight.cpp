#include "Fligh.h"

#include <sstream>
#include <iomanip>

Flight::Flight(int _flight_id, const std::string & data) : flight_id(_flight_id)
{
	std::istringstream sin(data);
	sin >> name >> dep_airport >> arr_airport >> plan_dep_time >> plan_arr_time >> delay >> propagated_delay;
}

bool Flight::can_be_follow_by(const Flight & ano) const
{
	Time evaluateTime(ano.plan_dep_time - this->plan_arr_time);
	return this->arr_airport == ano.dep_airport
		&& evaluateTime >= SpecialTime::MinMCT && evaluateTime <= SpecialTime::MaxMCT;
}

bool operator<(const Flight &left, const Flight &right)
{
	if (left.plan_dep_time != right.plan_dep_time)
		return left.plan_dep_time < right.plan_dep_time;
	else if (left.plan_arr_time != right.plan_arr_time)
		return left.plan_arr_time < right.plan_arr_time;
	else if (left.dep_airport != right.dep_airport)
		return left.dep_airport < right.dep_airport;
	else if (left.arr_airport != right.arr_airport)
		return left.arr_airport < right.arr_airport;
	else if (left.delay != right.delay)
		return left.delay < right.delay;
	else
		return false;
}

std::ostream & operator<<(std::ostream & os, const Flight & flight)
{
	os << flight.flight_id << ' ' << flight.name << ' ' << flight.dep_airport << ' ' << flight.arr_airport << '\t' <<
		flight.plan_dep_time << ' ' << flight.plan_arr_time << '\t' <<
		std::setw(4) << flight.delay.totalMins() << ' ' <<
		std::setw(4) << flight.propagated_delay.totalMins();
	return os;
}