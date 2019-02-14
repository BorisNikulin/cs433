# produced with the help of Matthew Hatch

cmake_minimum_required(VERSION 3.13)

project(hw01)

add_executable(ready_queue_test
	src/pcb.cpp
	src/pcb_table.cpp
	src/ready_queue.cpp
	src/test_ready_queue.cpp
)

target_include_directories(ready_queue_test
	PRIVATE
		src
		./../data-structs
		./../compat
)

target_compile_features(ready_queue_test
	PRIVATE
		cxx_std_11
)

add_custom_command(OUTPUT bench_ready_queue.csv
	COMMAND ready_queue_test
)

add_custom_command(OUTPUT report.tex ./figure/
	COMMAND cp ${PROJECT_SOURCE_DIR}/report.Rnw ./
	COMMAND R -e "knitr::knit('report.Rnw')"
	MAIN_DEPENDENCY report.Rnw
	DEPENDS bench_ready_queue.csv
	VERBATIM
	BYPRODUCTS
		./figure/
		report.Rnw
		report.aux
		report.fdb_latexmk
		report.fls
		report.log
		report.out
		report.toc
)

add_custom_command(OUTPUT report.pdf
	COMMAND latexmk -lualatex report.tex
	MAIN_DEPENDENCY report.tex
	DEPENDS ./figure/
)

add_custom_target(report
	DEPENDS report.pdf
)
