CCX = gcc
CXXFLAGS = -std=c++11 -Wall -O3 -lpthread
BIN = producer_consumer

.PHONY: all
all: report.pdf

main.o: main.cpp shared_stack.hpp
	$(CXX) $(CXXFLAGS) -c $<

$(BIN): main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

report.pdf: report.tex
	latexmk -lualatex "$<"

.PHONY: clean
clean:
	-latexmk -c
	-rm *.o
	-rm $(BIN)
