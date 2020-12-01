set term pdfcairo enhanced size 8in, 3in
set output "tex/figs/normal-dist.pdf"

set samples 1000
set xrange [-4:4]
set yrange [0:0.45]

set grid

set lmargin at screen 0.05
set rmargin at screen 0.95

F(x) = exp(-(x * x) / 2) / sqrt(2 * pi)

plot F(x) linecolor 'red' linewidth 3 notitle