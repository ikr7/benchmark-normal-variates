from pathlib import Path

input_path = Path('benchmark.txt')
output_path = Path('tex/benchmark-result.tex')

output_text = ''
output_text += '\\begin{tabular}{crrrr}\n'
output_text += '    \\toprule\n'
output_text += '    アルゴリズム & 実行時間 ($\\mu$s) & 速度 & $N_U$ \\\\\n'
output_text += '    \\midrule\n'

for line in input_path.read_text().split('\n')[1:-1]:
    algorithm, time, rate, samples = line.split('\t')
    time = float(time)
    rate = float(rate)
    samples = float(samples)
    output_text += f'    {algorithm} & {time :.0f} & {rate :.0f} & {samples} \\\\\n'

output_text += '    \\bottomrule\n'
output_text += '\\end{tabular}\n'

output_path.write_text(output_text)