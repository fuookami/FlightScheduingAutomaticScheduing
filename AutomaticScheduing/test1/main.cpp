#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <thread>
#include "Flight.h"

void print_flight_set(const FlightSet &flight_set);
FlightBunches generate_initial_solution_with_greedy(const FlightSet &flight_set);
void print_flight_bunches(FlightBunches &flight_bunches);

void generate_and_print_all_solution(const FlightSet &flight_set, const FlightSet::const_iterator &curr_it, FlightBunches curr_bunches);

std::ofstream fout("out2.txt");
int best = INT_MAX;

int main(void)
{
	FlightSet flight_set;

	std::ifstream fin("data2.txt");
	std::string line_data;
	while (getline(fin, line_data))
		flight_set.insert(Flight(flight_set.size(), line_data));

	print_flight_set(flight_set);

	FlightBunches flight_bunches(generate_initial_solution_with_greedy(flight_set));
	fout << std::endl << "------------------Greedy------------------"
		<< std::endl << std::endl;
	print_flight_bunches(flight_bunches);
	best = flight_bunches.total_delay.to_int();

	fout << std::endl << "---------------All Solution---------------"
		<< std::endl << std::endl;
	generate_and_print_all_solution(flight_set, flight_set.cbegin(), FlightBunches());

	system("pause");
	return 0;

}

void print_flight_set(const FlightSet &flight_set)
{
	for (const Flight &flight : flight_set)
		fout << flight << std::endl;
}

void print_flight_bunches(FlightBunches &flight_bunches)
{
	int i = 1;
	Time total_delay(0, 0);
	for (const FlightBunch &flight_bunch : flight_bunches.flight_bunches)
	{
		fout << "Aircraft" << i << ": " << std::endl;
		for (const Flight &flight : flight_bunch.flights)
		{
			fout << flight << std::endl;
		}
		fout << std::endl;
		total_delay += flight_bunch.total_delay;
		++i;
	}
	flight_bunches.total_delay = total_delay;
	fout << "Total delay: " << flight_bunches.total_delay.to_int() << std::endl;
}

FlightBunches generate_initial_solution_with_greedy(const FlightSet &flight_set)
{
	FlightBunches flight_bunches;
	for (const Flight &flight : flight_set)
	{
		int min_added_delay_order = -1;
		Time min_added_delay(INT_MAX, INT_MAX);
		bool has_bunch_can_follow_by(false);

		int i(0);
		for (FlightBunch &flight_bunch : flight_bunches.flight_bunches)
		{
			if (flight_bunch.flights.back().can_be_follow_by(flight))
			{
				Time this_added_delay(flight_bunch.added_delay_if_add(flight));

				if (this_added_delay < min_added_delay)
				{
					min_added_delay = this_added_delay;
					min_added_delay_order = i;
				}

				has_bunch_can_follow_by = true;
			}
			++i;
		}

		if (!has_bunch_can_follow_by)
		{
			FlightBunch new_flight_bunch;
			new_flight_bunch.add_flight(flight);
			flight_bunches.flight_bunches.push_back(std::move(new_flight_bunch));
		}
		else
		{
			flight_bunches.flight_bunches[min_added_delay_order].add_flight(flight);
		}
	}

	return std::move(flight_bunches);
}

void generate_and_print_all_solution(const FlightSet &flight_set, const FlightSet::const_iterator &curr_it, FlightBunches curr_bunches)
{
	static int solution_num = 0;
	if (curr_it == flight_set.cend())
	{
		int i = 1;
		Time total_delay(0, 0);
		for (const FlightBunch &flight_bunch : curr_bunches.flight_bunches)
		{
			total_delay += flight_bunch.total_delay;
			++i;
		}
		if (total_delay.to_int() < best)
		{
			++solution_num;
			fout << "------Solution" << solution_num << "------" << std::endl << std::endl;
			print_flight_bunches(curr_bunches);
			fout << std::endl;
		}
	}
	else if (curr_it != flight_set.cend())
	{
		const Flight &flight = *curr_it;
		std::deque<FlightBunch> &flight_bunches(curr_bunches.flight_bunches);
		
		//std::vector<std::thread> threads;
		for (int i(0), j(flight_bunches.size()); i != j; ++i)
		{
			const FlightBunch &curr_bunch(flight_bunches[i]);
			if (curr_bunch.flights.back().can_be_follow_by(flight))
			{
				FlightBunches curr_bunches_copy(curr_bunches);
				curr_bunches_copy.flight_bunches[i].add_flight(flight);

				FlightSet::const_iterator next_it(curr_it);
				++next_it;
				generate_and_print_all_solution(flight_set, next_it, std::move(curr_bunches_copy));
				//threads.push_back(std::thread(generate_and_print_all_solution, 
				//	flight_set, next_it, std::move(curr_bunches_copy)));
			}
		}

		if (flight_bunches.size() >= FlightBunches::NumberOfAircraft)
		{
			return;
		}
		else
		{
			FlightBunch new_flight_bunch;
			new_flight_bunch.add_flight(flight);
			flight_bunches.push_back(std::move(new_flight_bunch));
			
			FlightSet::const_iterator next_it(curr_it);
			++next_it;
			generate_and_print_all_solution(flight_set, next_it, std::move(curr_bunches));
		}
	}
}