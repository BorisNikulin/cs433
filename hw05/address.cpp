#include "address.h"

#include <istream>
#include <vector>

namespace memory
{
	std::vector<Address> loadMemoryReferenceFile(std::istream& is)
	{
		std::vector<Address> refs;
		unsigned int reference;

		while((is >> reference))
		{
			refs.push_back(Address{static_cast<Mode>(reference & 1), reference & ~1});
		}

		return refs;
	}

	std::ostream& operator<<(std::ostream& os, const Address ref)
	{
		return os << (ref.addr | static_cast<bool>(ref.mode));
	}
}

