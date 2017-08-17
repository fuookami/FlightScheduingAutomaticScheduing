#pragma once

#include <iostream>
#include <string>

class Time
{
	friend std::istream &operator >> (std::istream &is, Time &time);
	friend std::ostream &operator << (std::ostream &os, const Time &time);

	friend bool operator==(const Time &left, const Time &right);
	friend bool operator!=(const Time &left, const Time &right);
	friend bool operator<(const Time &left, const Time &right);
	friend bool operator>(const Time &left, const Time &right);
	friend bool operator<=(const Time &left, const Time &right);
	friend bool operator>=(const Time &left, const Time &right);

public:
	Time() : totalMin(0) {}
	Time(int mins) : totalMin(mins) {}
	Time(int hour, int min) : totalMin(hour * MinsOfOneHour + min) {}
	Time(const Time &ano) : totalMin(ano.totalMin) {}
	Time(const Time &&ano) : totalMin(ano.totalMin) {}

	Time &setHour(int hour) { totalMin = hour * MinsOfOneHour + totalMin % MinsOfOneHour; return *this; }
	Time &setMin(int min) { totalMin = MinsOfOneHour * (totalMin / MinsOfOneHour) + min; return *this; }
	Time &setHourMin(int hour, int min) { totalMin = hour *MinsOfOneHour + min; return *this; }

	int totalMins() const { return totalMin; }
	std::string toString() const;
	bool isNextDay() const { return totalMin >= MinsOfOneDay; }
	int hour() const { return totalMin / MinsOfOneHour % 24; }
	int min() const { return totalMin % MinsOfOneHour; }

	Time &operator=(const Time &ano);
	Time &operator=(const Time &&ano);
	Time operator+(const Time &ano) const;
	Time operator+(const int min) const;
	Time &operator+=(const Time &ano);
	Time &operator+=(const int min);
	Time operator-(const Time &ano) const;
	Time operator-(const int min) const;
	Time &operator-=(const Time &ano);
	Time &operator-=(const int min);
	Time operator*(const int times);
	Time &operator*=(const int times);

private:
	static const int MinsOfOneHour = 60;
	static const int HoursOfOneDay = 24;
	static const int MinsOfOneDay = MinsOfOneHour * HoursOfOneDay;

	int totalMin;
};

namespace SpecialTime
{
	static const Time ZeroTime(0, 0);
	static const Time MaxTime(INT_MAX);
	static const Time MinMCT(0, 45);
}