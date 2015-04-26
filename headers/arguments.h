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

#ifndef HEADERS_ARGUMENTS_H_
#define HEADERS_ARGUMENTS_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

template<typename T>
struct arguments { //parsed command line arguments
	string type;
	bool display_information = false;
	bool use_parallel = false;
	bool output_information = false;
	string information_path;
	bool input_data = false;
	string input_path;
	bool output_data = false;
	string output_path;
	bool random_input = false;
	long long int random_input_length = 1;
	T random_input_start = 0;
	T random_input_end = 1;
};

vector<string> convert_arguments(int argc, char* argv[]); // convert char* array into string vector

template<typename T>
bool argument_is_type(string argument); //check if argument is parsable to type T

template<typename T>
arguments<T> parse_arguments(int argc, char* argv[]); //parse the command line arguments into arguments structure

string get_data_type(int argc, char* argv[]); // get the type of the sorting data (-t parameter)

// include template definitions
#include "../sources/arguments.h"


#endif /* HEADERS_ARGUMENTS_H_ */
