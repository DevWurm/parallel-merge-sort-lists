/*
	 * License

	 * Copyright 2015 DevWurm

	 * This file is part of parallel_merge_sort_library.

	 *  parallel_merge_sort_library is free software: you can redistribute it and/or modify
	    it under the terms of the GNU General Public License as published by
	    the Free Software Foundation, either version 3 of the License, or
	    (at your option) any later version.

	    parallel_merge_sort_library is distributed in the hope that it will be useful,
	    but WITHOUT ANY WARRANTY; without even the implied warranty of
	    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	    GNU General Public License for more details.

	    You should have received a copy of the GNU General Public License
	    along with parallel_merge_sort_library.  If not, see <http://www.gnu.org/licenses/>.

	    Diese Datei ist Teil von parallel_merge_sort_library.

	    parallel_merge_sort_library ist Freie Software: Sie können es unter den Bedingungen
	    der GNU General Public License, wie von der Free Software Foundation,
	    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
	    veröffentlichten Version, weiterverbreiten und/oder modifizieren.

	    parallel_merge_sort_library wird in der Hoffnung, dass es nützlich sein wird, aber
	    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
	    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
	    Siehe die GNU General Public License für weitere Details.

	    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
	    Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
*/

#ifndef LIB_LIBPARALLELMERGESORT_H_
#define LIB_LIBPARALLELMERGESORT_H_


#include "../libcsv/libcsv.h"
#include <deque>
#include <chrono>
#include <string>
#include <ostream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <thread>
#include <cstdlib>
#include "../liberror/liberror.h"

using std::deque;
using std::string;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::stringstream;
using csv::csv_parser;
using csv::csv_creator;
using csv::csv_handler;
using std::ostream;
using std::istream;
using std::out_of_range;
using err::error;
using std::thread;
using std::ref;

namespace par_merge_sort {

template<typename T>
class data_row {
private:
	deque<T> data;
	string type;
	steady_clock::duration operation_time;
public:
	data_row(); //standard constructor
	data_row(deque<T>&& input); //move constructor from deque
	data_row(const deque<T>& input); //copy constructor from deque

	data_row<T>& operator=(deque<T>&& input); //move operator from deque
	data_row<T>& operator=(const deque<T>& input); //assign operator from deque

	template<typename S>
	friend data_row<S>& operator >>(csv_parser<S>& input, data_row<S>& target);
	csv_creator<T>& operator >>(csv_creator<T>& target);
	template<typename S>
	friend istream& operator >>(istream& input, data_row<S>& target);
	template<typename S>
	friend ostream& operator <<(ostream& output, data_row<S>& source);

	void merge_sort_data();
	void parallel_merge_sort_data();

	void set_data(deque<T>&& input); //set data from deque (move)
	void set_data(const deque<T>& input); //set data from deque
	void set_data(const string& csv_string); //set data from csv string

	deque<T> get_data() const; //get data as deque
	string get_type() const; //get type
	steady_clock::duration get_operation_time(string unit) const; //get operation time as duration
	steady_clock::duration get_operation_time() const; //get operation time as duration (milliseconds)
	string get_operation_time_string(string unit) const; //get operation time as string
	string get_operation_time_string() const; //get operation time as string (milliseconds)
	int get_data_length() const;

	void clear(); //delete data
};

template<typename T>
data_row<T>::data_row() {
	type = typeid(T).name();
}

template<typename T>
data_row<T>::data_row(deque<T>&& input): data(input) {
	type = typeid(T).name();
	input.clear();
}

template<typename T>
data_row<T>::data_row(const deque<T>& input): data(input) {
	type = typeid(T).name();
}

template<typename T>
	data_row<T>& data_row<T>::operator =(const deque<T>& input) {
	data = input;
}

template<typename T>
data_row<T>& data_row<T>::operator =(deque<T>&& input) {
	data = input;
	input.clear();
}

template<typename T>
data_row<T>& operator >>(csv_parser<T>& input, data_row<T>& target) {
	input >> target.data;
	return target;
}

template<typename T>
csv_creator<T>& data_row<T>::operator >>(csv_creator<T>& target) {
	data >> target;
	return target;
}

template<typename T>
istream& operator >>(istream& input, data_row<T>& target) {
	csv_handler<T> temp_handler;
	input >> temp_handler >> target.data;
	return input;
}

template<typename T>
ostream& operator <<(ostream& output, data_row<T>& source) {
	csv_handler<T> temp_handler;
	source.data >> temp_handler >> output;
	return output;
}

template<typename T>
void data_row<T>::set_data(const deque<T>& input) {
	data = input;
}

template<typename T>
void data_row<T>::set_data(deque<T>&& input) {
	data = input;
	input.clear();
}

template<typename T>
void data_row<T>::set_data(const string& csv_string) {
	csv_handler<T> temp_handler;
	temp_handler.set_csv_line(csv_string);
	set_data(temp_handler.get_parsed_line());
}

template<typename T>
deque<T> data_row<T>::get_data() const {
	return data;
}

template<typename T>
string data_row<T>::get_type() const {
	return type;
}

template<typename T>
void data_row<T>::clear() {
	data.clear();
}

template<typename T>
steady_clock::duration data_row<T>::get_operation_time(string unit) const { //return duration casted in given unit (default: milliseconds)
	if (unit == "h") {
		return duration_cast<hours>(operation_time);
	}
	else if (unit == "m") {
		return duration_cast<minutes>(operation_time);
	}
	else if (unit == "s") {
		return duration_cast<seconds>(operation_time);
	}
	else if (unit == "mic") {
		return duration_cast<microseconds>(operation_time);
	}
	else if (unit == "nan") {
		return duration_cast<nanoseconds>(operation_time);
	}
	else {
		return duration_cast<milliseconds>(operation_time);
	}
}

template<typename T>
steady_clock::duration data_row<T>::get_operation_time() const { //return duration casted in milliseconds
	return duration_cast<milliseconds>(operation_time);
}

template<typename T>
string data_row<T>::get_operation_time_string(string unit) const { //return duration casted in given unit (default: milliseconds)
	stringstream converter;
	if (unit == "h") {
		converter << duration_cast<hours>(operation_time).count();
		return converter.str();
	}
	else if (unit == "m") {
		converter << duration_cast<minutes>(operation_time).count();
		return converter.str();
	}
	else if (unit == "s") {
		converter << duration_cast<seconds>(operation_time).count();
		return converter.str();
	}
	else if (unit == "mic") {
		converter << duration_cast<microseconds>(operation_time).count();
		return converter.str();
	}
	else if (unit == "nan") {
		converter << duration_cast<nanoseconds>(operation_time).count();
		return converter.str();
	}
	else {
		converter << duration_cast<milliseconds>(operation_time).count();
		return converter.str();
	}
}

template<typename T>
string data_row<T>::get_operation_time_string() const { //return duration casted in milliseconds
	stringstream converter;
	converter << duration_cast<milliseconds>(operation_time).count();
	return converter.str();
}

template<typename T>
int data_row<T>::get_data_length() const {
	return data.size();
}

template<typename T>
void restack (deque<T>& source, deque<T>& target, int n){ //move n elements from the front of source to the end of target
	for (int i = 0; i <= n-1; i++) {
		try {
			target.push_back(source.at(0));
			source.pop_front();
		}
		catch (out_of_range& e) {
			throw(error(1, "Not enough entries in source list"));
		}
	}
}

template<typename T>
deque<T> merge_sort_merge(deque<T>& list1, deque<T>& list2) {
	if (list1.size()==1 && list2.size()==0) {
		return list1;
	}
	else {
		deque<T> listret;
		while (list1.size() > 0 && list2.size() >0) {
			if (list1.at(0)<list2.at(0)) {
				restack(list1, listret, 1);
			}
			else {
				restack(list2, listret, 1);
			}
		}
		if (list1.size()>0) {
			restack(list1, listret, list1.size());
		}
		else if (list2.size()>0) {
			restack(list2, listret, list2.size());
		}
		return listret;
	}

}

template<typename T>
void merge_sort(deque<T>& input, int basic_width = 1) {
	int input_size = input.size();
	for (int list_width = basic_width; list_width < input_size; list_width *= 2) {
		deque<T> buffer;
		restack(input, buffer, input.size());
		while (buffer.size() > 0) {
			deque<T> list1;
			deque<T> list2;
			if (buffer.size() >= list_width) {
				restack(buffer, list1, list_width);
			}
			else {
				restack(buffer, list1, buffer.size());
			}
			if (buffer.size() >= list_width) {
				restack(buffer, list2, list_width);
			}
			else {
				restack(buffer, list2, buffer.size());
			}
			deque<T> return_buffer = merge_sort_merge(list1, list2);
			restack(return_buffer, input, return_buffer.size());
		}

	}
}

template<typename T>
void data_row<T>::merge_sort_data() {
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	merge_sort(data);
	std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();
	operation_time = stop - start;
}

template<typename T>
struct thread_block {
	thread t;
	deque<T> data;
};


template<typename T>
void parallel_merge_sort(deque<T>& input) {
	int num_of_threads = thread::hardware_concurrency();
	deque<thread_block<T>> thread_list;
	int input_size = input.size();
	int basic_width = 1;

	while (num_of_threads > 0) {
		for (int i = 0; i <= num_of_threads-1; i++) {
			thread_list.emplace_back();
			if (i != num_of_threads-1) {
				restack(input,thread_list.at(i).data, ceil(input_size/num_of_threads));
				thread_list.at(i).t = thread(merge_sort<T>, ref(thread_list.at(i).data), ref(basic_width));
			}
			else {
				restack(input,thread_list.at(i).data, input.size());
				thread_list.at(i).t = thread(merge_sort<T>, ref(thread_list.at(i).data), ref(basic_width));
			}
		}
		for (int i = 0; i <= thread_list.size()-1; i++ ) {
			thread_list.at(i).t.join();
			restack(thread_list.at(i).data, input, thread_list.at(i).data.size());
		}
		thread_list.erase(thread_list.begin(), thread_list.end());
		basic_width = ceil(input_size/num_of_threads);
		num_of_threads = floor(num_of_threads/2);
	}

}

template<typename T>
void data_row<T>::parallel_merge_sort_data() {
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	parallel_merge_sort(data);
	std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();
	operation_time = stop - start;
}
}


#endif /* LIB_LIBPARALLELMERGESORT_H_ */
