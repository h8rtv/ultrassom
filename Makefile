VENDOR=vendor/eigen

all: compile run

compile:
	g++ -I $(VENDOR) src/main.cpp -o teste-eigen -O3

run:
	./teste-eigen
