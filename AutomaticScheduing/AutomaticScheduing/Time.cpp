#include "Time.h"

#include <iomanip>

std::istream & operator >> (std::istream & is, Time &time)
{
	static int hour(0), min(0);
	is >> hour >> min;
	time.totalMin = hour * min;
	return is;
}

std::ostream & operator<<(std::ostream & os, const Time & time)
{
	os << std::setfill('0') << std::setw(2) << time.hour() << ':' <<
		std::setw(2) << time.min() << std::setfill(' ');
	return os;
}

bool operator==(const Time & left, const Time & right)
{
	return left.totalMin == right.totalMin;
}

bool operator!=(const Time & left, const Time & right)
{
	return left.totalMin != right.totalMin;
}

bool operator<(const Time & left, const Time & right)
{
	return left.totalMin < right.totalMin;
}

bool operator>(const Time & left, const Time & right)
{
	return left.totalMin > right.totalMin;
}

bool operator<=(const Time & left, const Time & right)
{
	return left.totalMin <= right.totalMin;
}

bool operator>=(const Time & left, const Time & right)
{
	return left.totalMin >= right.totalMin;
}

std::string Time::toString() const
{
	std::string _hour(std::to_string(hour()));
	if (_hour.length() == 1)
		_hour.insert(0, "0");
	std::string _min(std::to_string(min()));
	if (_min.length() == 1)
		_min.insert(0, "0");
	if (isNextDay())
		return _hour + ":" + _min + "(ND)";
	else
		return _hour + ':' + _min;
}

Time Time::operator+(const Time & ano) const
{
	return Time(totalMin + ano.totalMin);
}

Time Time::operator+(const int min) const
{
	return Time(totalMin + min);
}

Time & Time::operator+=(const Time & ano)
{
	this->totalMin += ano.totalMin;
	return *this;
}

Time & Time::operator+=(const int min)
{
	this->totalMin += min;
	return *this;
}

Time Time::operator-(const Time & ano) const
{
	return Time(totalMin - ano.totalMin);
}

Time Time::operator-(const int min) const
{
	return Time(totalMin - min);
}

Time & Time::operator-=(const Time & ano)
{
	this->totalMin -= ano.totalMin;
	return *this;
}

Time & Time::operator-=(const int min)
{
	this->totalMin -= min;
	return *this;
}
