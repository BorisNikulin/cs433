CCX = gcc
CXXFLAGS = -std=c++11 -Wall -g

.PHONY: all
all: report.pdf

event.o: event.cpp event.h types.h
	$(CXX) $(CXXFLAGS) -c $<

event_queue.o: event_queue.cpp event_queue.h types.h process.h
	$(CXX) $(CXXFLAGS) -c $<

cpu.o: cpu.cpp cpu.h types.h event.h
	$(CXX) $(CXXFLAGS) -c $<

io_queue.o: io_queue.cpp types.h process.h
	$(CXX) $(CXXFLAGS) -c $<

fcfs_ready_queue.o: fcfs_ready_queue.cpp fcfs_ready_queue.h types.h
	$(CXX) $(CXXFLAGS) -c $<

sjf_ready_queue.o: sjf_ready_queue.cpp sjf_ready_queue.h types.h

stats.o: stats.cpp stats.h types.h
	$(CXX) $(CXXFLAGS) -c $<

main.o: main.cpp types.h event.h event_queue.h os.hpp sim_random.hpp fcfs_ready_queue.h sjf_ready_queue.h stats.h
	$(CXX) $(CXXFLAGS) -c $<

os_sim: main.o event.o event_queue.o stats.o cpu.o io_queue.o fcfs_ready_queue.o sjf_ready_queue.o
	$(CXX) $(CXXFLAGS) -o $@ $^

os_sim_10.csv: os_sim
	./$< 10

os_sim_20.csv: os_sim
	./$< 20

os_sim_100.csv: os_sim
	./$< 1000

report.tex: report.Rnw os_sim_10.csv os_sim_20.csv os_sim_100.csv
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
