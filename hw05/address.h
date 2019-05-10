#pragma once

#include <istream>
#include <iostream>
#include <vector>

namespace memory
{

	using addr_t = unsigned int;
	using page_id_t = unsigned int;
	using frame_id_t = unsigned int;
	using offset_t = unsigned int;

	enum class Mode : bool
	{
		READ = 0, WRITE = 1
	};

	struct Address
	{
		Mode mode;
		addr_t addr;
	};

	std::vector<Address> loadMemoryReferenceFile(std::istream& is);

	std::ostream& operator<<(std::ostream& os, const Address ref);
}
