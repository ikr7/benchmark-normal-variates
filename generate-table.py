from pathlib import Path
from math import log10

input_path = Path('benchmark.txt')
benchmark_output_path = Path('tex/benchmark-result.tex')
n_output_path = Path('tex/n-samples-benchmark.tex')

benchmark_output_text = ''
benchmark_output_text += '\\begin{tabular}{crrrr}\n'
benchmark_output_text += '    \\toprule\n'
benchmark_output_text += '    アルゴリズム & 実行時間 (ms) & 速度 \\tnote{a} & $N_U$ \\tnote{b} \\\\\n'
benchmark_output_text += '    \\midrule\n'

input_text = input_path.read_text().split('\n')

power = int(log10(int(input_text[0])))
n_output_path.write_text(str(power))

for line in input_text[2:-1]:
    algorithm, time, rate, samples = line.split('\t')
    time = float(time)
    rate = float(rate)
    samples = float(samples)
    benchmark_output_text += f'    {algorithm} & {time / 1000:.0f} & {rate :.0f} & {samples :.4f} \\\\\n'

benchmark_output_text += '    \\bottomrule\n'
benchmark_output_text += '\\end{tabular}\n'

benchmark_output_path.write_text(benchmark_output_text)