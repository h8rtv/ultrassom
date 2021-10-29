VENDOR=-Ivendor/eigen

all: compile run

compile:
	g++ $(VENDOR) src/main.cpp -o teste-eigen -O3 -std=c++17

run:
	./teste-eigen
