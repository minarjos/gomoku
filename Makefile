gomoku: main.cpp position.h position.cpp human.h human.cpp printer.h printer.cpp ai.h ai.cpp
	g++ -o main main.cpp -O3 -lcurses

clean:
	rm main

.PHONY: clean
