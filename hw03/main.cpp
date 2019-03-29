#include "types.h"
#include "event.h"
#include "event_queue.h"
#include "os.hpp"
#include "fcfs_ready_queue.h"
#include "stats.h"

#include <random>
#include <iostream>
#include <cstdlib>

#include "sum_count.hpp"

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

	const UnitDuration endTime = chrono::duration_cast<UnitDuration>(chrono::minutes(5));
	EventQueue eventQueue;
	OS<mt19937, FCFSReadyQueue> os(mt19937{});

	eventQueue.pushEvents(os.newProcessEvents(numProcesses));

	while(!eventQueue.empty() && eventQueue.time() < endTime)
	{
		eventQueue.pushEvents(os.handleEvent(eventQueue.popEvent()));
	}

	for(auto pidStats : os.osStats().processStats())
	{
		procid_t pid = pidStats.first;
		ProcessStats procStats = pidStats.second;

		cout << "------------\n"
			<< "Process ID: " << pid << "\n"
			<< "Arrival time: " << procStats.arrivalTime.count() << "\n"
			<< "CPU time: " << procStats.cpuTime.count() << "\n"
			<< "IO time: " << procStats.ioTime.count() << "\n"
			<< "Wait time: " << procStats.waitTime.count() << "\n";
	}

	cout << "------------\n";
	cout << endl;
	cout << "See report for analysis.\n";

	return 0;
}
