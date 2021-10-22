VENDOR=vendor/eigen-3.4.0

all: compile run

compile:
	g++ -I $(VENDOR) src/main.cpp -o teste-eigen -O3

run:
	./teste-eigen
