CCX = gcc
CXXFLAGS = -std=c++11 -Wall -g -lpthread

.PHONY: all
all: report.pdf

main.o: main.cpp shared_stack.hpp
	$(CXX) $(CXXFLAGS) -c $<

producer_consumer: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

report.pdf: report.tex
	latexmk -lualatex "$<"

.PHONY: clean
clean:
	-latexmk -c
	-rm *.tex
	-rm -rf figures
	-rm *.o
	-rm os_sim
