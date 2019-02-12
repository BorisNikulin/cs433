#include "os_types.h"
#include "pcb.h"
#include "pcb_table.h"
#include "ready_queue.h"

#include "../data-structs/Vector.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <fstream>

namespace test { namespace ready_queue
{
	using namespace std;
	using namespace os;
	using namespace os::proc;

	auto test() -> bool
	{
		bool success = true;

		ReadyQueue q1;
		PCBTable pcbTable;

		for(int i = 1; i <= 20; ++i)
		{
			PCB* pcb = new PCB;
			pcb->pid = i;
			pcb->priority = i;
			pcb->state = State::NEW;

			pcbTable.addPCB(pcb);
		}

		for(procid_t i : {5, 1, 8, 11})
		{
			PCB* pcb = pcbTable.at(i);
			pcb->state = State::READY;
			q1.insertProc(pcbTable.at(i));
		}

		PCB* pcb;

		cout << "Ready Queues as Binary Heaps:\n";
		q1.displayQueue();

		pcb = q1.removeHighestProc();
		pcb->state = State::RUNNING;
		success &= pcb->pid == 1;
		q1.displayQueue();
		cout << "\n";

		pcb = q1.removeHighestProc();
		pcb->state = State::RUNNING;
		success &= pcb->pid == 5;
		q1.displayQueue();
		cout << "\n";

		for(procid_t i : {3, 7, 2, 12, 9})
		{
			PCB* pcb = pcbTable.at(i);
			pcb->state = State::READY;
			q1.insertProc(pcbTable.at(i));
		}

		for(procid_t i : {2, 3, 7, 8, 9, 11, 12})
		{
			pcb = q1.removeHighestProc();
			pcb->state = State::READY;
			success &= pcb->pid == i;
			q1.displayQueue();
			cout << "\n";
		}

		cout << "Ready queue is " << (q1.empty() ? "not " : "") << "empty\n";
		success &= q1.empty();

		return success;
	}

	auto bench() -> void
	{
		ds::container::Vector<PCB*> pcbVec(20);
		ReadyQueue q1;
		PCBTable pcbTable;

		ofstream resultsCsv("bench_ready_queue.csv", ios::trunc);
		resultsCsv << "run, time\n";

		enum class Action : bool
		{
			REMOVE = 0, ADD = 1
		};

		std::default_random_engine gen;
		std::uniform_int_distribution<procid_t> distPriority(1, 50);
		std::uniform_int_distribution<int> distAction(0, 1);
		auto randPriority = [&](){return distPriority(gen);};
		auto randAction = [&](){return static_cast<Action>(distAction(gen));};

		for(int i = 1; i <= 20; ++i)
		{
			PCB* pcb = new PCB;
			pcb->pid = i;
			pcb->priority = i;
			pcb->state = State::NEW;

			pcbVec.push_back(pcb);
		}

		std::random_shuffle(begin(pcbVec), end(pcbVec));

		for(size_t i = 0; i < 10; ++i)
		{
			pcbVec[i]->priority = randPriority();
			pcbVec[i]->state = State::RUNNING;
			q1.insertProc(pcbVec[i]);
		}

		for(size_t i = 0; i < 20; ++i)
		{
			pcbTable.addPCB(pcbVec[i]);
		}

		cout << "Benchmark start\n";
		for(int n = 1; n <= 20; ++n)
		{
			using namespace std::chrono;

			auto start = high_resolution_clock::now();

			for(long i = 0; i < 1000000; ++i)
			{
				switch(randAction())
				{
					case Action::REMOVE:
						if(!q1.empty())
						{
							PCB* pcb = q1.removeHighestProc();
							pcb->state = State::READY;
						}
						break;
					case Action::ADD:
						if(q1.size() < 20)
						{
							PCB* cur = pcbTable.first();
							while(cur->state != State::READY && cur->state != State::NEW)
							{
								cur = cur->next;
							}
							cur->priority = randPriority();
							q1.insertProc(cur);
						}
						break;
				}
			}
			auto end = high_resolution_clock::now();
			auto elapsed = duration_cast<milliseconds>(end - start);

			resultsCsv << n << ", " << elapsed.count() << "\n";
			cout << "Run " << n << " complete\n";
		}
	}

} }


int main()
{
	using namespace std;

	bool testSuccess = test::ready_queue::test();
	cout << endl;
	test::ready_queue::bench();

	cout << "Test " << (testSuccess ? "passed" : "failed") << "\n";

	return !testSuccess;
}
