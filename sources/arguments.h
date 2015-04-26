/*
	 * License

	 * Copyright 2015 DevWurm

	 * This file is part of merge_sort_lists.

	 *  merge_sort_lists is free software: you can redistribute it and/or modify
	    it under the terms of the GNU General Public License as published by
	    the Free Software Foundation, either version 3 of the License, or
	    (at your option) any later version.

	    merge_sort_lists is distributed in the hope that it will be useful,
	    but WITHOUT ANY WARRANTY; without even the implied warranty of
	    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	    GNU General Public License for more details.

	    You should have received a copy of the GNU General Public License
	    along with merge_sort_lists.  If not, see <http://www.gnu.org/licenses/>.

	    Diese Datei ist Teil von merge_sort_lists.

	    merge_sort_lists ist Freie Software: Sie können es unter den Bedingungen
	    der GNU General Public License, wie von der Free Software Foundation,
	    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
	    veröffentlichten Version, weiterverbreiten und/oder modifizieren.

	    merge_sort_lists wird in der Hoffnung, dass es nützlich sein wird, aber
	    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
	    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
	    Siehe die GNU General Public License für weitere Details.

	    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
	    Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
*/

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include "../liberror/liberror.h"
#include "../headers/arguments.h"

using std::vector;
using std::string;
using std::cout;
using err::error;
using std::stringstream;
using std::type_info;

vector<string> convert_arguments(int argc, char* argv[]) { // convert char* array into string vector
	vector<string> return_vector;
	for (int i = 0; i <= argc-1; i++) {
		return_vector.push_back(argv[i]); //implicit conversion from char* to string
	}
	return return_vector;
}

template<typename T>
bool argument_is_type(string argument) { //check if argument is parsable to type T
	stringstream converter;
	T type_variable;
	converter << argument;
	converter >> type_variable; //try to convert argument into type_variable of type T through stringstream
	if (converter.fail()) {
		return false;
	}
	else {
		return true;
	}
}

template<typename T>
arguments<T> parse_arguments(int argc, char* argv[]) { //parse the command line arguments into arguments structure
	vector<string> args = convert_arguments(argc, argv);
	arguments<T> return_values;
	return_values.type = typeid(T).name();

	int i = 1;
	while (i <= args.size()-1) {
		if (args.at(i) == "-t" || args.at(i) == "--type") { //type argument, already parsed and used in T
			++i; //skip type parameter
		}
		else if (args.at(i) == "-d" || args.at(i) == "--display") { //display argument (display start, stop, list length
			return_values.display_information = true;				// and duration in command line
		}
		else if (args.at(i) == "-p" || args.at(i) == "--parallel") { //Use parallel merge sort version
			return_values.display_information = true;
		}
		else if (args.at(i) == "-O" || args.at(i) == "--output-information") {//information output argument (display start, stop, list length
			return_values.output_information = true;						 // and duration into file
			return_values.information_path = args.at(i+1); //get next argument as file path
			++i; //move to next argument
		}
		else if (args.at(i) == "-i" || args.at(i) == "--input") { //input file argument (read data from csv file)
			if (return_values.random_input == true) { //check if random argument already set
				cout << "Combination of random input and input file not possible, choose one method\n";
				throw error(2, "Incorrect arguments");
				break;
			}
			else {
				return_values.input_data = true;
				return_values.input_path = args.at(i+1); //get next argument as file path
				++i; //move to next argument
			}
		}
		else if (args.at(i) == "-o" || args.at(i) == "--output") {  //output file argument (read data from csv file)
			return_values.output_data = true;
			return_values.output_path = args.at(i+1); //get next argument as file path
			++i; //move to next argument
		}
		else if (args.at(i) == "-r" || args.at(i) == "--random-input") { //random argument (generate data randomly)
			if (return_values.input_data == true) { //check if input argument already set
				cout << "Combination of random input and input file not possible, choose one method\n";
				throw error(2, "Incorrect arguments");
			}
			else {
				return_values.random_input = true;
				if (argument_is_type<long long int>(args.at(i+1))) { //get next argument (if parsable to long long int
					stringstream converter(args.at(i+1));			// as list length
					converter >> return_values.random_input_length;
					++i; //move to next argument
				}
				if (argument_is_type<T>(args.at(i+1))) { //get next argument (if parsable to T) as random distribution start value
					stringstream converter(args.at(i+1));
					converter >> return_values.random_input_start; //convert argument to T
					++i; //move to next argument
				}
				if (argument_is_type<T>(args.at(i+1))) {//get next argument (if parsable to T) as random distribution end value
					stringstream converter(args.at(i+1));
					converter >> return_values.random_input_end; //convert argument to T
					++i; //move to next argument
				}

				if (return_values.random_input_end < return_values.random_input_start) { //check if start value is bigger then end value
					return_values.random_input_end = return_values.random_input_start +1;
				}

			}
		}
		else { //throw an error at unknown argument
			throw error(3, "Unknown argument '" + args.at(i) + "'");
			break;
		}
		++i; //move to next argument
	}

	if (return_values.input_data == false && return_values.random_input == false) { //throw an error when no data source is set
		cout << "Neither random input nor input file defined, choose one method\n";
		throw error(2, "Incorrect arguments");
	}

	return return_values;
}

string get_data_type(int argc, char* argv[]) {  // get the type of the sorting data (-t parameter)
	vector<string> args = convert_arguments(argc, argv);

	for (int i = 0; i <= args.size()-1; i++) {
		if ((args.at(i) == "-t" || args.at(i) == "--type") && i != args.size()-1) { //search type argument in arguments
			//return typeid name of the given types, if they are supported
			if (args.at(i+1)=="int" || args.at(i+1)=="short") {
				return typeid(int).name();
			}
			else if (args.at(i+1)=="long") {
				return typeid(long).name();
			}
			else if (args.at(i+1)=="double" || args.at(i+1)=="float") {
				return typeid(double).name();
			}
			else {
				throw error(5, "Unknown type '" + args.at(i+1) + "'");
				break;
			}
			break; //break if type argument is found
		}
		else if (i == args.size()-1) { //throw error if no type argument is set
			throw error(4, "No given type");
		}
	}
}
