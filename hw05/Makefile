CCX = g++
CXXFLAGS = -std=c++11 -Wall -g
BIN = paging_sim

.PHONY: all
all: report.pdf

address.o: address.cpp address.h
	$(CXX) $(CXXFLAGS) -c $<

fifo.o: fifo.cpp fifo.h
	$(CXX) $(CXXFLAGS) -c $<

lru.o: lru.cpp lru.h
	$(CXX) $(CXXFLAGS) -c $<

main.o: main.cpp address.h memory.hpp fifo.h random.hpp lru.h
	$(CXX) $(CXXFLAGS) -c $<

$(BIN): main.o address.o fifo.o lru.o
	$(CXX) $(CXXFLAGS) -o $@ $^

paging_sim.tsv: $(BIN) sim_runner.sh summary.awk
	./sim_runner.sh ./$(BIN)

report.tex: report.Rnw paging_sim.tsv
	R -e "knitr::knit('$<')"

report.pdf: report.tex
	latexmk -lualatex "$<"

.PHONY: clean
clean:
	-latexmk -c
	-rm *.tex
	-rm -rf figures
	-rm *.o
	-rm $(BIN)
