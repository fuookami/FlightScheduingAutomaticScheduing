#pragma once

#include "Time.h"
#include <deque>
#include <set>
#include <memory>

struct FlightInfo
{
	int flight_id;
	std::string name;

	std::string dep_airport;
	std::string arr_airport;

	Time plan_dep_time;
	Time plan_arr_time;

	Time delay;

	FlightInfo(int _flight_id, std::string _name, std::string _dep_airport, std::string _arr_airport, Time _plan_dep_time,
		Time _plan_arr_time, Time _delay, Time _propagated_delay) : name(_name), flight_id(_flight_id), dep_airport(_dep_airport),
		arr_airport(_arr_airport), plan_dep_time(_plan_dep_time), plan_arr_time(_plan_arr_time), delay(_delay) {};
	FlightInfo(const FlightInfo &ano) : name(ano.name), flight_id(ano.flight_id), dep_airport(ano.dep_airport), arr_airport(ano.arr_airport),
		plan_dep_time(ano.plan_dep_time), plan_arr_time(ano.plan_arr_time), delay(ano.delay) {};
	FlightInfo(int _flight_id, const std::string &data);

	bool canBeFollowedBy(const FlightInfo &ano) const;
	std::string toString(void) const;
};
bool operator<(const FlightInfo &lop, const FlightInfo &rop);
std::ostream &operator<<(std::ostream &os, const FlightInfo &flightInfo);

using FlightInfoSet = std::set<std::shared_ptr<FlightInfo>>;

class Flight
{
	friend bool operator<(const Flight &lop, const Flight &rop);
	friend std::ostream &operator<<(std::ostream &os, const Flight &flight);

public:
	Flight(std::shared_ptr<FlightInfo> _ptrInfo, const Time &_propagatedDelay = Time(0, 0)) : ptrInfo(_ptrInfo) {}

	bool canBeFollowedBy(const FlightInfo &anoInfo) const;
	bool canBeFollowedBy(const Flight &ano) const;

	std::string toString(void) const;
private:
	std::shared_ptr<FlightInfo> ptrInfo;
	Time propagatedDelay;
};

class FlightBunch
{
public:
	FlightBunch() : totalDelay(0, 0) {};

	bool addFlight(const Flight &flight);
	bool eraseFlight(const std::deque<Flight>::size_type i);
	bool eraseFlight(const std::deque<Flight>::iterator it);

	std::deque<Flight>::size_type size() const;

	std::deque<Flight>::iterator begin() const;
	std::deque<Flight>::const_iterator cbegin() const;
	std::deque<Flight>::iterator end() const;
	std::deque<Flight>::const_iterator cend() const;

private:
	void sortFlights(void);
	void calDelayTime(void);

	std::deque<Flight> flights;
	Time totalDelay;
};

class FlightPlan
{

};