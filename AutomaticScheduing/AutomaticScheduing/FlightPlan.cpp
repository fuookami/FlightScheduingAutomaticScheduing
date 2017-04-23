#include "FlightPlan.h"

#include <sstream>
#include <iomanip>

FlightInfo::FlightInfo(int _flight_id, const std::string & data) : id(_flight_id)
{
	std::istringstream sin(data);
	sin >> name >> depAirport >> arrAirport >> planDepTime >> planArrTime >> delay;
}

bool FlightInfo::canBeFollowedBy(const FlightInfo & ano) const
{
	Time evaluateTime(ano.planDepTime - this->planArrTime);
	return this->arrAirport == ano.depAirport && evaluateTime >= SpecialTime::MinMCT;
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
	if (lop.planDepTime != rop.planDepTime)
		return lop.planDepTime < rop.planDepTime;
	else if (lop.planArrTime != rop.planArrTime)
		return lop.planArrTime < rop.planArrTime;
	else if (lop.depAirport != rop.depAirport)
		return lop.depAirport < rop.depAirport;
	else if (lop.arrAirport != rop.arrAirport)
		return lop.arrAirport < rop.arrAirport;
	else if (lop.delay != rop.delay)
		return lop.delay < rop.delay;
	else
		return false;
}

std::ostream & operator<<(std::ostream & os, const FlightInfo & flightInfo)
{
	os << flightInfo.id << ' ' << flightInfo.name << ' ' << flightInfo.depAirport << ' ' << 
		flightInfo.arrAirport << '\t' << flightInfo.planDepTime << ' ' << flightInfo.planArrTime << '\t' <<
		std::setw(4) << flightInfo.delay.totalMins();
	return os;
}

bool operator<(const Flight &lop, const Flight &rop)
{
	return *lop.ptrInfo < *lop.ptrInfo;
}

std::ostream &operator<<(std::ostream &os, const Flight &flight)
{
	os << *flight.ptrInfo << ' ' << std::setw(4) << flight.propagatedDelay.totalMins() << std::endl;
}

const FlightInfo & Flight::info(void) const
{
	return *ptrInfo;
}

const Time & Flight::delay(void) const
{
	return propagatedDelay;
}

void Flight::setPropagatedDelayFollowed(const Flight & preFlight)
{
	propagatedDelay = preFlight.ptrInfo->delay + preFlight.propagatedDelay + preFlight.ptrInfo->planArrTime
		+ SpecialTime::MinMCT - ptrInfo->planDepTime;
	if (propagatedDelay < Time(0, 0))
		propagatedDelay.setHourMin(0, 0);
}

bool Flight::canBeFollowedBy(const FlightInfo & nextFlightInfo) const
{
	return ptrInfo->canBeFollowedBy(nextFlightInfo);
}

bool Flight::canBeFollowedBy(const Flight & nextFlight) const
{
	return ptrInfo->canBeFollowedBy(*nextFlight.ptrInfo);
}
