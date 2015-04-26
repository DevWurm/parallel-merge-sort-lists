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



