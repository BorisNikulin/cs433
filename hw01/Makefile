report.tex: report.Rnw
	R -e "knitr::knit('$<')"

report.pdf: report.tex
	latexmk -lualatex "$<"

.PHONY : clean
clean :
	-latexmk -c
	-rm *.tex
