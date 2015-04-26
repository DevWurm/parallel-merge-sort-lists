/*
 * sort.cpp
 *
 *  Created on: 20.04.2015
 *      Author: leo
 */

#include <string>
#include <typeinfo>
#include "liberror/liberror.h"
#include "headers/arguments.h"
#include "headers/process.h"

using namespace std;

int main(int argc, char* argv[]) {
	string type;
		try {
			type = get_data_type(argc, argv); //get typeid name of given type
		}
		catch (err::error& e) {
			e.output_error();
			exit(1);
		}

		try {
			//use the matching process version to type
			if (type == typeid(int).name()) {
				perform_sorting<int>(argc, argv);
			}
			else if (type == typeid(long).name()) {
				perform_sorting<long>(argc, argv);
			}
			else if (type == typeid(double).name()) {
				perform_sorting<double>(argc, argv);
			}
		}
		catch (err::error& e) {
			e.output_error();
			exit(2);
		}

		return 0;
}



