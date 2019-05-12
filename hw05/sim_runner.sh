#!/usr/bin/env bash

bin=$1

pagePowers=(8 8 13 13)
memPowers=(16 24 16 24)

for i in ${!pagePowers[@]}
do
	cmd="$bin ${pagePowers[$i]} ${memPowers[$i]}"
	echo "$cmd"
	$cmd > "tmp$i.tsv"
	awk -f summary.awk "tmp$i.tsv"
done

header=false

for tsv in $(ls tmp*.tsv)
do
	if [ "$header" = false ]
	then
		cp "$tsv" tmp.tsv
		header=true
	else
		tail -n +2 "$tsv" >> tmp.tsv
	fi
done

mv tmp.tsv paging_sim.tsv
gzip paging_sim.tsv
