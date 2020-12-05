bins = 50
samples_benchmark = 1000000
samples_histogram = 100000
compiler_options = -std=c++2a -Wall --pedantic-errors -O2

advent.pdf: figures tables
	echo "l($(samples_benchmark))/l(10)" | bc -l | cut -f1 -d. > tex/n-samples-benchmark.tex
	echo "l($(samples_histogram))/l(10)" | bc -l | cut -f1 -d. > tex/n-samples-histogram.tex
	cd tex && latexmk -quiet -r latexmkrc -C advent.tex
	cd tex && latexmk -quiet -r latexmkrc advent.tex
	cd tex && latexmk -quiet -r latexmkrc -c advent.tex
	rm tex/advent.bbl
	rm tex/advent.dvi
	mv tex/advent.pdf .

figures: histogram.txt
	mkdir -p tex/figs
	gnuplot histogram.plt
	gnuplot montypython.plt
	gnuplot normal-dist.plt

tables: benchmark.txt
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

.PHONY: result.txt benchmark.txt clean