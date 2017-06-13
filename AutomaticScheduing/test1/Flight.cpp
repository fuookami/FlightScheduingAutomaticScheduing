#include "Flight.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Flight::Flight(int _flight_id, const std::string & data) : id(_flight_id)
{
	std::istringstream sin(data);
	sin >> name >> depAirport >> arrAirport >> planDepTime >> planArrTime >> delay >> propagated_delay;
}

bool Flight::can_be_follow_by(const Flight & ano) const
{
	Time evaluateTime(ano.planDepTime - this->planArrTime);
	return this->arrAirport == ano.depAirport 
		&& evaluateTime >= SpecialTime::MinMCT && evaluateTime <= SpecialTime::MaxMCT;
}

bool operator<(const Flight &left, const Flight &right)
{
	if (left.planDepTime != right.planDepTime)
		return left.planDepTime < right.planDepTime;
	else if (left.planArrTime != right.planArrTime)
		return left.planArrTime < right.planArrTime;
	else if (left.depAirport != right.depAirport)
		return left.depAirport < right.depAirport;
	else if (left.arrAirport != right.arrAirport)
		return left.arrAirport < right.arrAirport;
	else if (left.delay != right.delay)
		return left.delay < right.delay;
	else
		return false;
}

std::ostream & operator<<(std::ostream & os, const Flight & flight)
{
	os << flight.id << ' ' << flight.name << ' ' << flight.depAirport << ' ' << flight.arrAirport << '\t' << 
		flight.planDepTime << ' ' << flight.planArrTime << '\t' <<
 		std::setw(4) << flight.delay.to_int() << ' ' << 
		std::setw(4) << flight.propagated_delay.to_int();
	return os;
}

void FlightBunch::add_flight(const Flight & flight)
{
	Time propagated_delay = this->added_delay_if_add(flight) - flight.delay;
	flights.push_back(flight);
	if (flights.size() != 1)
	{
		flights.back().propagated_delay = std::move(propagated_delay);
	}
	total_delay += propagated_delay;
}

Time FlightBunch::added_delay_if_add(const Flight & flight) const
{
	if (this->flights.empty())
		return flight.delay;
	else
	{
		Time propagated_delay(0, 0);
		Flight last_flight(flights.back());
		propagated_delay = last_flight.delay + last_flight.propagated_delay +
			last_flight.planArrTime + SpecialTime::MinMCT - flight.planDepTime;
		if (propagated_delay < Time(0, 0))
			propagated_delay = Time(0, 0);
		return propagated_delay + flight.delay;
	}
}
