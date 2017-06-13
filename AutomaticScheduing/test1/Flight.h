#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <set>
#include <memory>
#include "Time.h"

struct Flight 
{
	int id;
	std::string name;

	std::string depAirport;
	std::string arrAirport;
	
	Time planDepTime;
	Time planArrTime;
	
	Time delay;
	Time propagated_delay;

	Flight() {};
	Flight(int _flight_id, std::string _name, std::string _dep_airport, std::string _arr_airport, Time _plan_dep_time, 
		Time _plan_arr_time, Time _delay, Time _propagated_delay) : name(_name), id(_flight_id), depAirport(_dep_airport), 
		arrAirport(_arr_airport), planDepTime(_plan_dep_time), planArrTime(_plan_arr_time), delay(_delay),
		propagated_delay(_propagated_delay) {}
	Flight(const Flight &ano) : name(ano.name), id(ano.id), depAirport(ano.depAirport), arrAirport(ano.arrAirport), 
		planDepTime(ano.planDepTime), planArrTime(ano.planArrTime), delay(ano.delay), 
		propagated_delay(ano.propagated_delay) {}
	Flight(int _flight_id, const std::string &data);

	bool can_be_follow_by(const Flight &ano) const;
};

bool operator<(const Flight &left, const Flight &right);
std::ostream &operator<<(std::ostream &os, const Flight &flight);

using FlightSet = std::set<Flight>;

struct FlightBunch
{
	FlightBunch() : total_delay(0, 0) {};

	void add_flight(const Flight &flight);
	Time added_delay_if_add(const Flight &flight) const;

	std::deque<Flight> flights;
	Time total_delay;
};

struct FlightBunches
{
	static const int NumberOfAircraft = 40;

	FlightBunches() : total_delay(0, 0) {};

	std::deque<FlightBunch> flight_bunches;
	std::set<int> flight_id_set;
	Time total_delay;
};

using FlightBunchSet = std::set<FlightBunches>;