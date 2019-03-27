CCX = gcc
CXXFLAGS = -std=c++11 -Wall -g

.PHONY: all
all: report.pdf

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

os_sim: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

os_sim.csv: os_sim
	./$<

report.tex: report.Rnw bench_ready_queue.csv
	R -e "knitr::knit('$<')"

report.pdf: report.tex
	latexmk -lualatex "$<"

.PHONY: clean
clean:
	-latexmk -c
	-rm *.tex
	-rm -rf figures
	-rm *.o
	-rm os_sim
