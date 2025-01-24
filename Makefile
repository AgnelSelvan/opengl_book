
.PHONY: prompt

prompt:
	g++ -std=c++20 1.2.2.cpp -o main; \
	clear; \
	./main

all: prompt