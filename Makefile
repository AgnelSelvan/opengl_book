
.PHONY: prompt

prompt:
	g++ -std=c++20 main.cpp -o main; \
	clear; \
	./main

all: prompt