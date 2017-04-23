#pragma once

#include "Time.h"
#include <deque>
#include <set>
#include <map>
#include <memory>

struct FlightInfo
{
	unsigned int id;
	std::string name;

	std::string depAirport;
	std::string arrAirport;

	Time planDepTime;
	Time planArrTime;

	Time delay;

	FlightInfo(int ID, std::string Name, std::string DepAirport, std::string ArrAirport, Time PlanDepTime,
		Time PlanArrTime, Time Delay) : name(Name), id(ID), depAirport(DepAirport), arrAirport(ArrAirport), 
		planDepTime(PlanDepTime), planArrTime(PlanArrTime), delay(Delay) {};
	FlightInfo(const FlightInfo &ano) : name(ano.name), id(ano.id), depAirport(ano.depAirport), arrAirport(ano.arrAirport),
		planDepTime(ano.planDepTime), planArrTime(ano.planArrTime), delay(ano.delay) {};
	FlightInfo(int _flight_id, const std::string &data);

	bool canBeFollowedBy(const FlightInfo &ano) const;
	std::string toString(void) const;
};
bool operator<(const FlightInfo &lop, const FlightInfo &rop);
std::ostream &operator<<(std::ostream &os, const FlightInfo &flightInfo);

using FlightInfoSet = std::set<std::shared_ptr<FlightInfo>>;
using FlightInfoMap = std::map<unsigned int, std::shared_ptr<FlightInfo>>;

class Flight
{
	friend bool operator<(const Flight &lop, const Flight &rop);
	friend std::ostream &operator<<(std::ostream &os, const Flight &flight);

public:
	Flight(std::shared_ptr<FlightInfo> _ptrInfo, const Time &_propagatedDelay = Time(0, 0)) : ptrInfo(_ptrInfo) {}

	const FlightInfo &info(void) const;
	const Time &delay(void) const;

	void setPropagatedDelayFollowed(const Flight &preFlight);
	bool canBeFollowedBy(const FlightInfo &nextFlightInfo) const;
	bool canBeFollowedBy(const Flight &nextFlight) const;

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
	std::deque<Flight> &flights(void) const;
	std::set<unsigned int> &ids(void) const;

	std::deque<Flight>::iterator begin() const;
	std::deque<Flight>::const_iterator cbegin() const;
	std::deque<Flight>::iterator end() const;
	std::deque<Flight>::const_iterator cend() const;

private:
	void sortFlights(void);
	void calDelayTime(void);

	std::set<unsigned int> flightId;
	std::deque<Flight> flight;
	Time totalDelay;
};