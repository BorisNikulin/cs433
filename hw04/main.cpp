#include "shared_stack.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include <pthread.h>

const size_t BUFFER_SIZE = 10;
concurrent::SharedStack<int, BUFFER_SIZE> stack;
std::mt19937 gen;
std::uniform_int_distribution<int> dist;

bool run = true;

void* produce(void*)
{
	while(run)
	{
		stack.emplace(dist(gen));
	}

	return nullptr;
}

void* consume(void*)
{
	while(run)
	{
		stack.pop();
	}

	return nullptr;
}

void printHelp(const char* progName)
{
	using namespace std;

	cout << progName
		<< " <sleep time> <num producers> <num consumers>\n"
		<< "\t<sleep time> - How long the main thread sleeps before teriminating (in secondsi, >0.0)\n"
			"\t<num producers> - The number of producer threads (>0)\n"
			"\t<num consumers> - The numbr of consumer threads (>0)\n";
}

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace std::chrono;

	using namespace concurrent;

	duration<double> sleepSeconds;
	int numProducers;
	int numConsumers;

	if(argc == 4)
	{
		sleepSeconds = duration<double>(atof(argv[1]));
		numProducers = atoi(argv[2]);
		numConsumers = atoi(argv[3]);

		bool doExit = false;

		if(sleepSeconds <= duration<double>::zero())
		{
			cout << "<sleep time> needs to be greater than 0\n";
			doExit = true;
		}
		if(numProducers <= 0)
		{
			cout << "<num producers> needs to be greater than 0\n";
			doExit = true;
		}
		if(numConsumers <= 0)
		{
			cout << "<num consumers> needs to be greater than 0\n";
			doExit = true;
		}

		if(doExit)
		{
			return 1;
		}
	}
	else
	{
		printHelp(argv[0]);
		return 0;
	}

	vector<pthread_t> threads;
	threads.reserve(numProducers + numConsumers);

	for(int i = 0; i < numProducers; ++i)
	{
		pthread_t t;
		pthread_create(&t, nullptr, produce, nullptr);
		threads.push_back(t);
	}

	for(int i = 0; i < numConsumers; ++i)
	{
		pthread_t t;
		pthread_create(&t, nullptr, consume, nullptr);
		threads.push_back(t);
	}

	this_thread::sleep_for(sleepSeconds);
	run = false;
	for_each(threads.begin(), threads.end(), [](pthread_t thread){ pthread_join(thread, nullptr); });

	return 0;
}
