{
	if($6 == "reference")
	{
		refCount[$5] += 1
	}

	if($6 == "fault")
	{

		faultCount[$5] += 1
	}

	if($6 == "replace")
	{

		replaceCount[$5] += 1
	}

	if($6 == "flush")
	{

		flushCount[$5] += 1
	}
}
END {
	print cmd

	formatStr = "\t%10s references %'10d, faults %'10d, replacements %'10d, flushes %'10d\n"
	printf formatStr, "FIFO:",  refCount["fifo"], faultCount["fifo"], replaceCount["fifo"], flushCount["fifo"]
	printf formatStr, "RNG:mt19937:",
		refCount["rng:mt19937"],
		faultCount["rng:mt19937"],
		replaceCount["rng:mt19937"],
		flushCount["rng:mt19937"]
	printf formatStr, "RNG:minst:",
		refCount["rng:minstd_rand"],
		faultCount["rng:minstd_rand"],
		replaceCount["rng:minstd_rand"],
		flushCount["rng:minstd_rand"]
	printf formatStr, "LRU:", refCount["lru"], faultCount["lru"], replaceCount["lru"], flushCount["lru"]
}
