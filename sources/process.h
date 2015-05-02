/*
	 * License

	 * Copyright 2015 DevWurm

	 * This file is part of merge_bubble_sort_compare.

	 *  merge_bubble_sort_compare is free software: you can redistribute it and/or modify
	    it under the terms of the GNU General Public License as published by
	    the Free Software Foundation, either version 3 of the License, or
	    (at your option) any later version.

	    merge_bubble_sort_compare is distributed in the hope that it will be useful,
	    but WITHOUT ANY WARRANTY; without even the implied warranty of
	    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	    GNU General Public License for more details.

	    You should have received a copy of the GNU General Public License
	    along with merge_bubble_sort_compare.  If not, see <http://www.gnu.org/licenses/>.

	    Diese Datei ist Teil von merge_bubble_sort_compare.

	    merge_bubble_sort_compare ist Freie Software: Sie können es unter den Bedingungen
	    der GNU General Public License, wie von der Free Software Foundation,
	    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
	    veröffentlichten Version, weiterverbreiten und/oder modifizieren.

	    merge_bubble_sort_compare wird in der Hoffnung, dass es nützlich sein wird, aber
	    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
	    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
	    Siehe die GNU General Public License für weitere Details.

	    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
	    Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
*/

#include "../headers/process.h"

#include <fstream>
#include <iostream>
#include <deque>
#include <thread>
#include "../liberror/liberror.h"
#include "../libmergesort/libmergesort.h"
#include "../headers/arguments.h"
#include "../headers/random_list.h"

using par_merge_sort::data_row;
using namespace std;

template<typename T>
void perform_sorting(int argc, char* argv[]) {
	arguments<T> user_arguments;
	data_row<T> sort;
	ofstream output_file;
	ofstream information_file;


	//parse command line arguments
	try {
		user_arguments = parse_arguments<T>(argc, argv);
	}
	catch (err::error& e) {
		throw e;
	}

	//open output file if option is set
	if (user_arguments.output_data) {
		output_file.open(user_arguments.output_path);
		if (output_file.fail()) {
			throw err::error(7, "Output file not found");
		}
	}

	//open information output file if option is set
	if (user_arguments.output_information) {
		information_file.open(user_arguments.information_path);
		if (information_file.fail()) {
			throw err::error(8, "Information output file not found");
		}
	}

	//get data and perform sort and output
	if (user_arguments.input_data) { //use input file if option is set

		string buffer;

		ifstream input_file (user_arguments.input_path); //open input file
		if (input_file.fail()) {
			throw err::error(6, "Input file not found");
		}

		while (getline(input_file, buffer)) { //read each line into buffer
			sort.set_data(buffer); //read buffer into compare

			if (user_arguments.display_information) { //sort with verbose output if option is set
				cout << "List length: " << sort.get_data_length() << '\n';
				if (user_arguments.use_parallel) { //display number of processors
					cout << "Using " << thread::hardware_concurrency() << " Threads\n";
				}
				cout << "Sorting started\n";
				if (user_arguments.use_parallel) {
					sort.parallel_merge_sort_data(); //sort parallel if option is set
				}
				else {
					sort.merge_sort_data(); //else sort sequential
				}
				cout << "Sorting ended\n";
				cout << "Operation time: " << sort.get_operation_time_string() << "ms\n";
			}
			else { //else sort with non-verbose version
				if (user_arguments.use_parallel) {
					sort.parallel_merge_sort_data(); //sort parallel if option is set
				}
				else {
					sort.merge_sort_data(); //else sort sequential
				}
			}

			if (user_arguments.output_data) { //write sorted data into output if option is set
				output_file << sort << '\n';
			}

			if (user_arguments.output_information) { //write data length and operation times into csv file
				information_file << sort.get_data_length() << ',' << sort.get_operation_time_string() << '\n';
			}
		}
		input_file.close(); //close input file
	}
	else { //otherwise use random input
		deque<T> random_data = generate_random_list<T>(user_arguments.random_input_length, user_arguments.random_input_start, user_arguments.random_input_end); //generate random list
		sort.set_data(random_data); //assign random list to compare data

		if (user_arguments.display_information) { //sort with verbose output if option is set
			cout << "List length: " << sort.get_data_length() << '\n';
			if (user_arguments.use_parallel) { //display number of processors
				cout << "Using " << thread::hardware_concurrency() << " Threads\n";
			}
			cout << "Sorting started\n";
			if (user_arguments.use_parallel) {
				sort.parallel_merge_sort_data(); //sort parallel if option is set
			}
			else {
				sort.merge_sort_data(); //else sort sequential
			}
			cout << "Sorting ended\n";
			cout << "Operation time: " << sort.get_operation_time_string() << "ms\n";
		}
		else { //else sort with non-verbose version
			if (user_arguments.use_parallel) {
				sort.parallel_merge_sort_data(); //sort parallel if option is set
			}
			else {
				sort.merge_sort_data(); //else sort sequential
			}
		}

		if (user_arguments.output_data) { //write sorted data into output if option is set
			output_file << sort << '\n';
		}

		if (user_arguments.output_information) { //write data length and operation times into csv file
			information_file << sort.get_data_length() << ',' << sort.get_operation_time_string() << '\n';
		}
	}

	//close open files
	if (output_file.is_open()) {
		output_file.close();
	}

	if (information_file.is_open()) {
		information_file.close();
	}
}
