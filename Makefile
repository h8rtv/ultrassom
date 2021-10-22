VENDOR=vendor/eigen-3.4.0

all: compile run

compile:
	g++-10 -I $(VENDOR) src/main.cpp -o teste-eigen -O3 -std=c++2a

run:
	./teste-eigen
