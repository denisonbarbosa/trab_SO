main: src/Shell.h src/ShellController.h
	mkdir -p bin/
	g++ -Wall -pedantic -o bin/main src/main.cpp src/Shell.cpp src/ShellController.cpp