gomoku: main.cpp position.h position.cpp human.h human.cpp printer.h printer.cpp ai.h ai.cpp game.h game.cpp player.h
	g++ -o main main.cpp -O3 -lcurses -Wall

clean:
	rm main

.PHONY: clean
