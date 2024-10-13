
.PHONY: prompt

prompt:
	@echo "Select one to run:\n1. OOPS in C++"
	@read option; \
	echo "You have selected, $$option!"; \
	if [ $$option -eq 1 ]; then \
		FILENAME="1.2.1.cpp"; \
	else \
		echo "Invalid option!"; \
		exit 1; \
	fi; \
	g++ -std=c++20 $$FILENAME -o main; \
	clear; \
	./main

all: prompt