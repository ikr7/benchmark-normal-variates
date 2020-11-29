set term pdfcairo enhanced size 4in, 3in

set style fill solid
set boxwidth 0.1

set xrange [-3.5:3.5]
set yrange [0:0.5]

set output "tex/figs/hist-clt.pdf"
plot "histogram.txt" using 1:2 with boxes lc 'red' notitle

set output "tex/figs/hist-box-muller.pdf"
plot "histogram.txt" using 1:3 with boxes lc 'red' notitle

set output "tex/figs/hist-polar.pdf"
plot "histogram.txt" using 1:4 with boxes lc 'red' notitle

set output "tex/figs/hist-kinderman.pdf"
plot "histogram.txt" using 1:5 with boxes lc 'red' notitle

set output "tex/figs/hist-montypython.pdf"
plot "histogram.txt" using 1:6 with boxes lc 'red' notitle

set output "tex/figs/hist-ziggurat.pdf"
plot "histogram.txt" using 1:7 with boxes lc 'red' notitle