set term pdfcairo enhanced size 4in, 2in
set output "tex/figs/monty-python.pdf"

set samples 1000
set xrange [0:3]
set yrange [0:0.45]

F(x) = exp(-(x * x) / 2) / sqrt(2 * pi)
G(x) = 1 / (2 * B) - (F(B - x) - 1 / (2 * B))
B = 2.29
A = sqrt(log(2 * B * B / pi))

set arrow from B,0 to B,0.5/B lc rgb 'dark-grey' nohead
set arrow from A,0 to A,0.5/B lc rgb 'dark-grey' nohead
set arrow from 0,0.5/B to B,0.5/B lc rgb 'dark-grey' nohead

set label 1 center at 0.5,0.1 "A"
set label 2 center at 1.5,0.05 "B"
set label 3 center at 0.4,0.3 "C"
set label 4 center at 2,0.15 "C'"

set arrow from 2.75,0.05 to (B+(2.5-B)/2),0.01 nohead
set label 5 center at 2.75,0.065 "D"

set arrow from 1.5,0.3 to 1.2,0.205 nohead
set label 6 center at 1.5,0.315 "D'"

set arrow from 0.4,0.175 to 0,0.5/B nohead
set label 7 left at 0.4,0.175 "0.5/b"

set label 8 center at B,-0.02 "b"
set label 9 center at A,-0.02 "a"

plot \
    F(x) dashtype (10, 10) linecolor 'black' title 'f(x)', \
    [0:B] (G(x) <= 0.5/B ? G(x) : 1/0) linecolor 'black' notitle