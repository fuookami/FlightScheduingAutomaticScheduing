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

	void setPropagatedDelayFollow(const Flight &preFlight);
	Time getPropagatedDelayIfFollowedBy(const std::shared_ptr<FlightInfo> ptrNextInfo) const;
	Time getPropagatedDelayIfFollow(const std::shared_ptr<FlightInfo> ptrPrepInfo, const Time &prepPropagatedDealy) const;
	bool canBeFollowedBy(const FlightInfo &nextFlightInfo) const;
	bool canBeFollowedBy(const Flight &nextFlight) const;

	std::string toString(void) const;

private:
	static Time calPropagatedDealy(const std::shared_ptr<FlightInfo> ptrPrepInfo, const Time &prepPropagatedDealyconst, std::shared_ptr<FlightInfo> ptrThisInfo);

private:
	std::shared_ptr<FlightInfo> ptrInfo;
	Time propagatedDelay;
};

class FlightBunch
{
public:
	FlightBunch() : totalDelay(0, 0) {};

	bool addFlight(const std::shared_ptr<FlightInfo> pFlightInfo);
	std::deque<Flight>::iterator findFlighById(const unsigned int id);
	bool eraseFlight(const std::deque<Flight>::size_type i);
	bool eraseFlight(const std::deque<Flight>::iterator it);

	Time addedDelayIfAddFlight(const std::shared_ptr<FlightInfo> pFlightInfo) const;
	const Time &delay(void) const;

	Flight &operator[](const int i);
	std::deque<Flight>::size_type size() const;
	std::deque<Flight> &flights(void);
	std::set<unsigned int> &ids(void);

	std::deque<Flight>::iterator begin();
	std::deque<Flight>::const_iterator cbegin() const;
	std::deque<Flight>::iterator end();
	std::deque<Flight>::const_iterator cend() const;

private:
	void calDelayTime(void);

	std::set<unsigned int> flightId;
	std::deque<Flight> flight;
	Time totalDelay;
};