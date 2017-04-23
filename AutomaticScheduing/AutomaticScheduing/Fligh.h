#pragma once

#include "Time.h"
#include <set>

struct Flight
{
	int flight_id;
	std::string name;

	std::string dep_airport;
	std::string arr_airport;

	Time plan_dep_time;
	Time plan_arr_time;

	Time delay;
	Time propagated_delay;

	Flight() {};
	Flight(int _flight_id, std::string _name, std::string _dep_airport, std::string _arr_airport, Time _plan_dep_time,
		Time _plan_arr_time, Time _delay, Time _propagated_delay) : name(_name), flight_id(_flight_id), dep_airport(_dep_airport),
		arr_airport(_arr_airport), plan_dep_time(_plan_dep_time), plan_arr_time(_plan_arr_time), delay(_delay),
		propagated_delay(_propagated_delay) {}
	Flight(const Flight &ano) : name(ano.name), flight_id(ano.flight_id), dep_airport(ano.dep_airport), arr_airport(ano.arr_airport),
		plan_dep_time(ano.plan_dep_time), plan_arr_time(ano.plan_arr_time), delay(ano.delay),
		propagated_delay(ano.propagated_delay) {}
	Flight(int _flight_id, const std::string &data);

	bool can_be_follow_by(const Flight &ano) const;
};

bool operator<(const Flight &left, const Flight &right);
std::ostream &operator<<(std::ostream &os, const Flight &flight);

using FlightSet = std::set<Flight>;