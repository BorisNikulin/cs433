#include "PriorityQueue.hpp"
#include "Vector.hpp"

#include <iomanip>
#include <iostream>
#include <queue>

namespace ds::test::container::priority_queue
{
	// tests for a stable priority queue
	auto test() -> bool
	{
		using namespace std;
		using namespace ds::container;

		using Pair = pair<int, int>;

		cout << __FILE__ << endl;

		auto comp = [](auto p1, auto p2){ return p1.second > p2.second; };

		std::priority_queue<Pair, Vector<Pair>, decltype(comp)> refPq(comp);
		PriorityQueue<Pair, Vector<Pair>, decltype(comp)> pq(comp);

		auto input = {3, 7, 3, 3, 4, 2};

		bool success = true;

		int i = 0;
		for(auto x : input)
		{
			refPq.push({i, x});
			pq.push({i, x});
			++i;
		}

		while(!pq.empty())
		{
			auto pqTop = pq.take();
			success &= refPq.top() == *pqTop;
			auto [id1, num1] = *pqTop;
			auto [id2, num2] = refPq.top();

			cout << setw(8)<< "pq: (" << id1
				<< ", " << num1 << ")" << endl;

			cout << setw(8) <<"refPq: (" << id2
				<< ", " << num2 << ")" << endl;

			refPq.pop();
		}
		cout << endl;

		return success;
	}
}
