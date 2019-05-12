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
	formatStr = "\t%10s references %'10d, faults %'10d, replacements %'10d, flushes %'10d\n"
	printf formatStr, "FIFO:",  refCount["fifo"], faultCount["fifo"], replaceCount["fifo"], flushCount["fifo"]
	printf formatStr, "Random:", refCount["random"], faultCount["random"], replaceCount["random"], flushCount["random"]
	printf formatStr, "LRU:", refCount["lru"], faultCount["lru"], replaceCount["lru"], flushCount["lru"]
}
