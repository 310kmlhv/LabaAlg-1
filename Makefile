all: main

main:
	g++ -std=c++11 main.cpp -o main

rebuild: clean main

clean:
	rm -rf *.txt main
