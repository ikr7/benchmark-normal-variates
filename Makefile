bins = 50
samples = 100000
compiler_options = -std=c++2a -Wall --pedantic-errors

advent.pdf: figures tables
	echo "l($(samples))/l(10)" | bc -l | cut -f1 -d. > tex/n_samples.tex
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
	gnuplot normal_dist.plt

tables: benchmark.txt
	python generate_table.py

histogram.txt: histogram result.txt
	cat result.txt | ./histogram $(bins)

histogram: histogram.cpp
	g++ $(compiler_options) -o histogram histogram.cpp

result.txt: benchmark
	./benchmark $(samples) 1

benchmark: benchmark.cpp
	g++ $(compiler_options) -o benchmark benchmark.cpp

clean:
	rm -f benchmark
	rm -f histogram
	rm -f result.txt
	rm -f benchmark.txt
	rm -f histogram.txt
	rm -f -r tex/figs
	rm -f tex/benchmark_result.tex
	rm -f tex/n_samples.tex
	rm -f advent.pdf

.PHONY: result.txt clean