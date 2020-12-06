bins = 50
samples_benchmark = 1000000
samples_histogram = 100000
compiler_options = -std=c++2a -Wall --pedantic-errors -O2 -fopenmp

tables = tex/benchmark-result.tex
figures = tex/figs/histograms.pdf tex/figs/monty-python.pdf tex/figs/normal-dist.pdf

advent.pdf: $(figures) $(tables) tex/advent.tex
	cd tex && latexmk -quiet -r latexmkrc -C advent.tex
	cd tex && latexmk -quiet -r latexmkrc advent.tex
	cd tex && latexmk -quiet -r latexmkrc -c advent.tex
	rm tex/advent.bbl
	rm tex/advent.dvi
	mv tex/advent.pdf .

$(figures): histogram.txt
	head -n1 histogram.txt > tex/n-samples-histogram.tex
	mkdir -p tex/figs
	gnuplot histogram.plt
	gnuplot montypython.plt
	gnuplot normal-dist.plt

$(tables): benchmark.txt
	python generate-table.py

histogram.txt: histogram result.txt
	cat result.txt | ./histogram $(bins)

histogram: histogram.cpp
	$(CXX) $(compiler_options) -o histogram histogram.cpp

result.txt: benchmark
	./benchmark $(samples_histogram) 1

benchmark.txt: benchmark
	./benchmark $(samples_benchmark) 0

benchmark: benchmark.cpp
	$(CXX) $(compiler_options) -o benchmark benchmark.cpp

clean:
	$(RM) benchmark
	$(RM) histogram
	$(RM) result.txt
	$(RM) benchmark.txt
	$(RM) histogram.txt
	$(RM) -r tex/figs
	$(RM) tex/benchmark-result.tex
	$(RM) tex/n-samples-benchmark.tex
	$(RM) tex/n-samples-histogram.tex
	$(RM) advent.pdf

.PHONY: clean