CCX = g++
CXXFLAGS = -std=c++11 -Wall -g
BIN = paging_sim

.PHONY: all
all: report.pdf

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

$(BIN): main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

paging_sim.csv: $(BIN)
	./$(BIN)

report.tex: report.Rnw paging_sim.csv
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
