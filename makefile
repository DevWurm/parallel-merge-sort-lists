all:
	g++ sort.cpp liberror/liberror.a -std=c++11 -pthread -o sort
run:
	g++ sort.cpp liberror/liberror.a -std=c++11 -pthread -o builds/run/sort
debug:
	g++ sort.cpp liberror/liberror.a -std=c++11 -pthread -o builds/debug/sort
