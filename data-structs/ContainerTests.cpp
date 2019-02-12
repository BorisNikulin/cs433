#include <iostream>

#include "List.hpp"
#include <list>

namespace ds
{
	namespace test
	{
		namespace container
		{
			namespace list
			{
				int test()
				{
					using namespace std;
					using namespace ds::container;

					std::list<int> refL1 = {1,2,3,4,5,6};
					List<int> myL1 = {1,2,3,4,5,6};
					List<int> myL2 = std::move(myL1);
					//for(int i = 0; i < 6; ++i)
					//{
					//while(!myL1.empty())
					//{

						//auto x = myL1.take_back();
						//myL2.give_back(std::move(x));
						//myL2.push_back(myL1.front());
						//cout << myL1.front() << ", ";
						//myL1.pop_front();
					//}

					while(!myL2.empty())
					{
						cout << myL2.front() << ", ";
						//myL2.pop_front();
						myL2.erase(myL2.cbegin());
						cout << "(size: " << myL2.size() << ")";
					}
					cout << endl << myL1.empty();
					cout << endl;
					return 0;
				}
			}
		}
	}
}

int main()
{
	using namespace ds::test::container;
	return list::test();
}
