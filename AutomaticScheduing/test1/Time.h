#pragma once

#include <iostream>

struct Time 
{
	int hour;
	int min;
	Time() : hour(0), min(0) {}
	Time(int _int) : hour(_int / 60), min(_int % 60) {}
	Time(int _hour, int _min) : hour(_hour), min(_min) {}
	Time(const Time &ano) : hour(ano.hour), min(ano.min) {}
	
	int to_int() const { return hour * 60 + min; }

	Time operator+(const Time &ano) const;
	Time operator+(const int min) const;
	Time &operator+=(const Time &ano);
	Time &operator+=(const int min);
	Time operator-(const Time &ano) const;
	Time operator-(const int min) const;
	Time &operator-=(const Time &ano);
	Time &operator-=(const int min);
};

std::istream &operator >> (std::istream &is, Time &time);
std::ostream &operator << (std::ostream &os, const Time &time);
std::string to_string(const Time &time);

bool operator==(const Time &left, const Time &right);
bool operator!=(const Time &left, const Time &right);
bool operator<(const Time &left, const Time &right);
bool operator>(const Time &left, const Time &right);
bool operator<=(const Time &left, const Time &right);
bool operator>=(const Time &left, const Time &right);

namespace SpecialTime
{
	const Time MinMCT(0, 45);
	const Time MaxMCT(8, 00);
}