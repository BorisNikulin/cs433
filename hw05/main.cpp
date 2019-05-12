#include "address.h"
#include "memory.hpp"
#include "fifo.h"
#include "random.hpp"
#include "lru.h"

#include <random>
#include <cstdlib>
#include <ostream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

namespace memory
{

	void printHelp(const char* progName)
	{
		using namespace std;

		cout << progName
			<< " <page num bits> <mem num bits>\n"
			<< "\t<page num bits> - The number of bits for the page id.\n"
				"\t\tCorresponds to the page size of 2^<page num bits>.\n"
				"\t\tMust be a number in the range [8, 13].\n"
			<< "\t<mem num bits> - The number of bits for the physical address.\n"
			 "\t\tCorresponds to the memory size of 2^<ram num bits>.\n"
			 "\t\tMust be a number greater than 0.\n";
	}

	struct Log
	{
		std::chrono::microseconds time;
		int numPages;
		int pageSizeBytes;
		int memorySizeBytes;
		std::string replacementAlgorithm;
		Event e;
	};

	void writeHeaderTsv(std::ostream& os)
	{
		os << "time"
			<< '\t' << "num_pages"
			<< '\t' << "page_size_bytes"
			<< '\t' << "memory_size_bytes"
			<< '\t' << "replacement_algorithm"
			<< '\t' << "event"
			<< '\t' << "page"
			<< '\n';
	}

	void writeLogTsv(std::ostream& os, const Log& l)
	{
		os << l.time.count()
			<< '\t' << l.numPages
			<< '\t' << l.pageSizeBytes
			<< '\t' << l.memorySizeBytes
			<< '\t' << l.replacementAlgorithm
			<< '\t' << eventTypeToString(l.e.type)
			<< '\t' << l.e.pageId
			<< '\n';
	}

	template<typename Memory, typename ForwardIt>
	void inline simulateMemory(
		Memory& mem,
		const std::chrono::time_point<std::chrono::high_resolution_clock> startTime,
		const int numPages,
		const int pageSizeBytes,
		const int memorySizeBytes,
		const std::string replacementAlgorithm,
		std::ostream& os,
		ForwardIt refsBegin,
		ForwardIt refsEnd)
	{
		using namespace std::chrono;

		for(; refsBegin != refsEnd; ++refsBegin)
		{
			auto addrEvents = mem.reference(*refsBegin);
			for(Event& e : addrEvents.second)
			{
				Log log{
					duration_cast<microseconds>(e.timestamp - startTime),
						numPages,
						pageSizeBytes,
						memorySizeBytes,
						replacementAlgorithm,
						e};
				writeLogTsv(os, log);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace std::chrono;
	using namespace memory;

	constexpr int logicalAddressNumBits = 27;

	int pageOffsetNumBits;
	int memNumBits;

	if(argc == 3)
	{
		pageOffsetNumBits = atoi(argv[1]);
		memNumBits = atoi(argv[2]);

		bool doExit = false;

		if(pageOffsetNumBits < 8 || pageOffsetNumBits > 13)
		{
			doExit = true;
			cout << "<page num bits> must be a number in the range [8, 13]\n";
		}
		if(memNumBits < 1)
		{
			doExit = true;
			cout << "<memory num bits> must be a number greater than 0\n";
		}
		if(pageOffsetNumBits >= memNumBits)
		{
			doExit = true;
			cout << "<page num num bits> must be greater than to <memory num bits>\n";
		}

		if(doExit)
		{
			return 1;
		}
	}
	else
	{
		printHelp(argv[0]);
		return 1;
	}

	int pageIdNumBits = logicalAddressNumBits - pageOffsetNumBits;
	int numPages = 1 << pageIdNumBits;
	int pageSizeBytes = 1 << pageOffsetNumBits;
	int memorySizeBytes = 1 << memNumBits;

	ifstream references("references.txt");
	//istringstream references("0 1 4 4 1 6 2");
	vector<Address> refs = loadMemoryReferenceFile(references);

	Memory<replacement::FIFO> memoryFifo(pageOffsetNumBits, numPages, memNumBits);
	Memory<replacement::Random<mt19937>> memoryRandomMersenne(pageOffsetNumBits, numPages, memNumBits, mt19937{}, numPages);
	Memory<replacement::Random<minstd_rand>> memoryRandomMinstd(pageOffsetNumBits, numPages, memNumBits,minstd_rand{}, numPages);
	Memory<replacement::LRU> memoryLru(pageOffsetNumBits, numPages, memNumBits);

	writeHeaderTsv(cout);

	simulateMemory(
		memoryFifo,
		high_resolution_clock::now(),
		numPages,
		pageSizeBytes,
		memorySizeBytes,
		"fifo",
		cout,
		refs.begin(),
		refs.end());

	simulateMemory(
		memoryRandomMersenne,
		high_resolution_clock::now(),
		numPages,
		pageSizeBytes,
		memorySizeBytes,
		"rng:mt19937",
		cout,
		refs.begin(),
		refs.end());

	simulateMemory(
		memoryRandomMinstd,
		high_resolution_clock::now(),
		numPages,
		pageSizeBytes,
		memorySizeBytes,
		"rng:minstd_rand",
		cout,
		refs.begin(),
		refs.end());

	simulateMemory(
		memoryLru,
		high_resolution_clock::now(),
		numPages,
		pageSizeBytes,
		memorySizeBytes,
		"lru",
		cout,
		refs.begin(),
		refs.end());

	return 0;
}
