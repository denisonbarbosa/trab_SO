main: src/Shell.h src/ShellController.h
	mkdir -p bin/
	g++ -o bin/main src/main.cpp src/Shell.cpp src/ShellController.cpp