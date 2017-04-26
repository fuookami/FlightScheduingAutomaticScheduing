#include "FlightBunch.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

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
	return os;
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

std::string Flight::toString(void) const
{
	std::ostringstream sout;
	sout << *this;
	std::string &str(sout.str());
	return std::move(str);
}

bool FlightBunch::addFlight(const std::shared_ptr<FlightInfo> pFlightInfo)
{
	if (flight.empty())
	{
		flight.push_back(pFlightInfo);
		return true;
	}
	else if (pFlightInfo->canBeFollowedBy(flight.front().info()))
	{
		flight.emplace_front(Flight(pFlightInfo));
		flightId.insert(pFlightInfo->id);
		calDelayTime();
	}
	else if (flight.back().canBeFollowedBy(pFlightInfo))
	{
		flight.emplace_back(Flight(pFlightInfo));
		flightId.insert(pFlightInfo->id);
		calDelayTime();
	}
	else
	{
		bool flag(false);
		for (std::deque<Flight>::size_type i(0), j(flight.size() - 1); i != j; ++i)
		{
			bool prep(flight[i].canBeFollowedBy(pFlightInfo));
			bool sufix(pFlightInfo->canBeFollowedBy(flight[i + 1].info()));

			if (prep && sufix)
			{
				flight.insert(flight.begin() + i, std::move(Flight(pFlightInfo)));
				flag = true;
			}
		}

		if (flag)
		{
			flightId.insert(pFlightInfo->id);
			calDelayTime();
		}

		return flag;
	}
}

std::deque<Flight>::iterator FlightBunch::findFlighById(const unsigned int id)
{
	return std::find_if(flight.begin(), flight.end(),
		[id](const Flight &flight)->bool
	{
		return flight.info().id == id;
	});
}

bool FlightBunch::eraseFlight(const std::deque<Flight>::size_type i)
{
	std::deque<Flight>::iterator ptr(flight.begin() + (unsigned int)i);
	flightId.erase(ptr->info().id);
	flight.erase(ptr);
	calDelayTime();
	return true;
}

bool FlightBunch::eraseFlight(const std::deque<Flight>::iterator it)
{
	flightId.erase(it->info().id);
	flight.erase(it);
	calDelayTime();
	return true;
}

const Time & FlightBunch::delay(void) const
{
	return totalDelay;
}

Flight & FlightBunch::operator[](const int i)
{
	return flight[i];
}

std::deque<Flight>::size_type FlightBunch::size() const
{
	return flight.size();
}

std::deque<Flight>& FlightBunch::flights(void)
{
	return flight;
}

std::set<unsigned int>& FlightBunch::ids(void)
{
	return flightId;
}

std::deque<Flight>::iterator FlightBunch::begin()
{
	return flight.begin();
}

std::deque<Flight>::const_iterator FlightBunch::cbegin() const
{
	return flight.cbegin();
}

std::deque<Flight>::iterator FlightBunch::end()
{
	return flight.end();
}

std::deque<Flight>::const_iterator FlightBunch::cend() const
{
	return flight.cend();
}

void FlightBunch::calDelayTime(void)
{
	totalDelay.setHourMin(0, 0);

	for (unsigned int i(1), j(flight.size()); i != j; ++i)
	{
		flight[i].setPropagatedDelayFollowed(flight[i - 1]);
		totalDelay += flight[i].delay();
	}
}
