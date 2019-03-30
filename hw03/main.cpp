#include "types.h"
#include "event.h"
#include "event_queue.h"
#include "os.hpp"
#include "fcfs_ready_queue.h"
#include "sjf_ready_queue.h"
#include "stats.h"

#include <random>
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <fstream>
#include <string>

void printStats(std::unordered_map<sim::procid_t, sim::ProcessStats>& procStats)
{
	using namespace std;
	using namespace sim;

	for(auto pidStats : procStats)
	{
		procid_t pid = pidStats.first;
		ProcessStats& procStats = pidStats.second;

		cout << "------------\n"
			<< "Process ID: " << pid << "\n"
			<< "Arrival time: " << procStats.arrivalTime.count() << "\n"
			<< "CPU time: " << procStats.cpuTime.count() << "\n"
			<< "IO time: " << procStats.ioTime.count() << "\n"
			<< "Wait time: " << procStats.waitTime.count() << "\n";
	}

	cout << "------------\n";
}

void writeCSVData(
	std::ostream& out,
	const char* schedulerType,
	std::unordered_map<sim::procid_t, sim::ProcessStats>& procStats)
{
	using namespace sim;

	for(auto pidStats : procStats)
	{
		procid_t pid = pidStats.first;
		ProcessStats& procStats = pidStats.second;

		out << schedulerType << ", "
			<< pid << ", "
			<< procStats.arrivalTime.count() << ", "
			<< procStats.finishTime.count() << ", "
			<< procStats.cpuTime.count() << ", "
			<< procStats.ioTime.count() << ", "
			<< procStats.waitTime.count() << "\n";
	}
}

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace sim;

	size_t numProcesses;

	if(argc == 2)
	{
		int num = atoi(argv[1]);
		if(num < 1)
		{
			cout << "Number of processes given must be 1 or greater.\n";
			return 1;
		}
		numProcesses = static_cast<size_t>(num);
	}
	else
	{
		cout << "Usage: " << argv[0] << " <num processes>\n";
		cout << "\tnum processes - Number of total processes for the simulation. Must be 1 or greater.\n";
		return 1;
	}

	cout << "OS Sim by Boris Nikulin.\n"
		<< "Running with " << numProcesses << " processes.\n";

	string outFilePath = "os_sim_";
	outFilePath += to_string(numProcesses) + ".csv";
	ofstream outCsv(outFilePath, ios::trunc);
	outCsv << "scheduler, pid, arrival, finish, cpu, io, wait\n";

	const UnitDuration endTime = chrono::duration_cast<UnitDuration>(chrono::minutes(5));

	EventQueue fcfsEventQueue;
	EventQueue sjfEventQueue;;

	OS<mt19937, FCFSReadyQueue> fcfsOS(mt19937{});
	OS<mt19937, SJFReadyQueue> sjfOS(mt19937{});

	fcfsEventQueue.pushEvents(fcfsOS.newProcessEvents(numProcesses));
	sjfEventQueue.pushEvents(sjfOS.newProcessEvents(numProcesses));

	while(!fcfsEventQueue.empty() && fcfsEventQueue.time() < endTime)
	{
		fcfsEventQueue.pushEvents(fcfsOS.handleEvent(fcfsEventQueue.popEvent()));
	}

	while(!sjfEventQueue.empty() && sjfEventQueue.time() < endTime)
	{
		sjfEventQueue.pushEvents(sjfOS.handleEvent(sjfEventQueue.popEvent()));
	}

	cout << "Times are in ms.\n\n";

	cout << "FCFS\n";
	printStats(fcfsOS.osStats().processStats());
	writeCSVData(outCsv, "FCFS", fcfsOS.osStats().processStats());
	cout << endl;

	cout << "SJF\n";
	printStats(sjfOS.osStats().processStats());
	writeCSVData(outCsv, "SJF", sjfOS.osStats().processStats());
	cout << endl;

	cout << "See report for analysis.\n";

	return 0;
}
