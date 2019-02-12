CCX = gcc
CXXFLAGS = -std=c++11 -Wall -O3

.PHONY: all
all: report.pdf

pcb.o: pcb.cpp os_types.h pcb.h
	$(CXX) $(CXXFLAGS) -c $<

pcb_table.o: pcb_table.cpp ./../data-structs/UnorderedMap.hpp  pcb.h pcb_table.h
	$(CXX) $(CXXFLAGS) -c $<

ready_queue.o: ready_queue.cpp ./../data-structs/PriorityQueue.hpp pcb.h
	$(CXX) $(CXXFLAGS) -c $<

ready_queue_test: ./../data-structs/Vector.hpp pcb.o pcb_table.o ready_queue.o test_ready_queue.cpp
	$(CXX) $(CXXFLAGS) -o $@ pcb.o pcb_table.o ready_queue.o test_ready_queue.cpp

bench_ready_queue.csv: ready_queue_test
	./$<

report.tex: report.Rnw bench_ready_queue.csv
	R -e "knitr::knit('$<')"

report.pdf: report.tex
	latexmk -lualatex "$<"

.PHONY: clean
clean:
	-latexmk -c
	-rm *.tex
	-rm *.o
	-rm ready_queue_test
