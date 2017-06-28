#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <thread>
#include "Flight.h"

void print_flight_set(const FlightSet &flight_set);
FlightBunches generate_initial_solution_with_greedy();
void print_flight_bunches(FlightBunches &flight_bunches);

void generate_and_print_all_solution(unsigned long index, FlightBunches curr_bunches);

std::ofstream fout("out2.txt");
int best = INT_MAX;
FlightSet flight_set;
FlightBunches bestFlightBunches;
static const unsigned int threadNumArg = 16;

int main(void)
{
	std::ifstream fin("data2.txt");
	std::string line_data;
	while (getline(fin, line_data))
		flight_set.push_back(Flight(flight_set.size(), line_data));

	print_flight_set(flight_set);

	FlightBunches flight_bunches(generate_initial_solution_with_greedy());
	fout << std::endl << "------------------Greedy------------------"
		<< std::endl << std::endl;
	print_flight_bunches(flight_bunches);
	bestFlightBunches = flight_bunches;
	best = flight_bunches.total_delay.to_int();

	fout << std::endl << "---------------All Solution---------------"
		<< std::endl << std::endl;
	generate_and_print_all_solution(0, FlightBunches());

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
	std::cout << "Total delay: " << flight_bunches.total_delay.to_int() << std::endl;
}

FlightBunches generate_initial_solution_with_greedy()
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

void generate_and_print_all_solution(unsigned long index, FlightBunches curr_bunches)
{
	static int solution_num = 0;
	if (index == flight_set.size())
	{
		int i = 1;
		Time total_delay(0, 0);
		for (const FlightBunch &flight_bunch : curr_bunches.flight_bunches)
		{
			total_delay += flight_bunch.total_delay;
			++i;
		}
		curr_bunches.total_delay = total_delay;
		if (bestFlightBunches.total_delay.to_int() < best)
			best = bestFlightBunches.total_delay.to_int();;

		if (total_delay.to_int() < best)
		{
			best = total_delay.to_int();
			bestFlightBunches = curr_bunches;
			++solution_num;
			fout << "------Solution" << solution_num << "------" << std::endl << std::endl;
			print_flight_bunches(curr_bunches);
			fout << std::endl;
		}
	}
	else if (index != flight_set.size())
	{
		const Flight &flight = flight_set[index];;
		std::deque<FlightBunch> &flight_bunches(curr_bunches.flight_bunches);
		
		std::vector<std::thread> threads;
		for (int i(0), j(flight_bunches.size()); i != j; ++i)
		{
			const FlightBunch &curr_bunch(flight_bunches[i]);
			if (curr_bunch.flights.back().can_be_follow_by(flight))
			{
				FlightBunches curr_bunches_copy(curr_bunches);
				curr_bunches_copy.flight_bunches[i].add_flight(flight);

				if (index < threadNumArg)
				{
					threads.push_back(std::thread(generate_and_print_all_solution,
						index + 1, std::move(curr_bunches_copy)));
				}
				else
				{
					generate_and_print_all_solution(index + 1, curr_bunches_copy);
				}
				
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

			if (index < threadNumArg)
			{
				threads.push_back(std::thread(generate_and_print_all_solution,
					index + 1, curr_bunches));
			}
			else
			{
				generate_and_print_all_solution(index + 1, curr_bunches);
			}
		}

		for (unsigned long i(0), j(threads.size()); i != j; ++i)
			threads[i].join();
	}
}