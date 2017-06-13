#include "Time.h"

#include <iomanip>

std::istream & operator >> (std::istream & is, Time &time)
{
	is >> time.hour >> time.min;
	return is;
}

std::ostream & operator<<(std::ostream & os, const Time & time)
{
	os << std::setfill('0') << std::setw(2) << time.hour << ':' <<
		std::setw(2) << time.min << std::setfill(' ');
	return os;
}

std::string to_string(const Time & time)
{
	std::string _hour(to_string(time.hour));
	if (_hour.length() == 1)
		_hour.insert(0, "0");
	std::string _min(to_string(time.min));
	if (_min.length() == 1)
		_min.insert(0, "0");
	return _hour + ':' + _min;
}

bool operator==(const Time & left, const Time & right)
{
	return left.hour == right.hour && left.min == right.min;
}

bool operator!=(const Time & left, const Time & right)
{
	return left.hour != right.hour || left.min != right.min;
}

bool operator<(const Time & left, const Time & right)
{
	return left.hour * 60 + left.min < right.hour * 60 + right.min;
}

bool operator>(const Time & left, const Time & right)
{
	return left.hour * 60 + left.min > right.hour * 60 + right.min;
}

bool operator<=(const Time & left, const Time & right)
{
	return left.hour * 60 + left.min <= right.hour * 60 + right.min;
}

bool operator>=(const Time & left, const Time & right)
{
	return left.hour * 60 + left.min >= right.hour * 60 + right.min;
}

Time Time::operator+(const Time & ano) const
{
	Time ret(this->hour + ano.hour, this->min + ano.min);
	if (ret.min >= 60) 
	{
		ret.min -= 60;
		++ret.hour;
	}
	return ret;
}

Time Time::operator+(const int min) const
{
	return *this + Time(0, min);
}

Time & Time::operator+=(const Time & ano)
{
	this->hour += ano.hour;
	this->min += ano.min;
	if (this->min >= 60) 
	{
		this->min -= 60;
		++this->hour;
	}
	return *this;
}

Time & Time::operator+=(const int min)
{
	this->min += min;
	if (this->min >= 60) 
	{
		this->min -= 60;
		++this->hour;
	}
	return *this;
}

Time Time::operator-(const Time & ano) const
{
	Time ret(this->hour - ano.hour, this->min - ano.min);
	if (ret.min < 0) 
	{
		ret.min += 60;
		--ret.hour;
	}
	return ret;
}

Time Time::operator-(const int min) const
{
	return *this - Time(0, min);
}

Time & Time::operator-=(const Time & ano)
{
	this->min -= ano.min;
	this->hour -= ano.hour;
	if (this->min < 0)
	{
		this->min += 60;
		--this->hour;
	}
	return *this;
}

Time & Time::operator-=(const int min)
{
	this->min -= min;
	if (this->min < 0)
	{
		this->min += 60;
		--this->hour;
	}
	return *this;
}
