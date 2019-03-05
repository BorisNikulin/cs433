CCX = g++
CXXFLAGS = -std=c++11 -Wall -g #-O3

.PHONY: all
all: report.pdf

shell.o: shell.cpp parser.h command.h
	$(CXX) $(CXXFLAGS) -c $<

parser.o: parser.cpp parser.h command.h program.h single_program.hpp
	$(CXX) $(CXXFLAGS) -c $<

built_in.o: built_in.cpp built_in.h
	$(CXX) $(CXXFLAGS) -c $<

command.o: command.cpp program.h built_in.h
	$(CXX) $(CXXFLAGS) -c $<

single_program.o: single_program.cpp single_program.hpp command.h
	$(CXX) $(CXXFLAGS) -c $<

test: main.cpp shell.h shell.o parser.o command.o built_in.o single_program.o
	$(CXX) $(CXXFLAGS) -o $@ $< shell.o parser.o command.o built_in.o single_program.o

report.pdf: report.tex
	latexmk -lualatex "$<"

.PHONY: clean
clean:
	-latexmk -c
	-rm *.o
