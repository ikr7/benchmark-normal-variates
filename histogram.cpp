#include <bits/stdc++.h>

using namespace std;

int main (int argc, char* argv[]) {
    
    size_t bins = stoi(argv[1]);
    double min = -3.5;
    double max = 3.5;

    size_t n, n_samplers;
    cin >> n >> n_samplers;

    vector<vector<size_t>> hists(n_samplers);
    vector<size_t> sample_counts(n_samplers);

    for (size_t sampler_index = 0; sampler_index < n_samplers; sampler_index++) {
        hists[sampler_index].resize(bins);
        sample_counts[sampler_index] = 0;
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t sampler_index = 0; sampler_index < n_samplers; sampler_index++) {
            double z;
            cin >> z;
            if (min < z && z < max) {
                size_t idx = floor(bins * (z - min) / (max - min));
                hists[sampler_index][idx]++;
                sample_counts[sampler_index]++;
            }
        }
    }

    ofstream histogram_of("histogram.txt", ios_base::out);

    for (size_t i = 0; i < bins; i++) {
        histogram_of << (min + (i + 0.5) * (max - min) / bins) << "\t";
        for (size_t sampler_index = 0; sampler_index < n_samplers; sampler_index++) {
            histogram_of << (hists[sampler_index][i] / (double)sample_counts[sampler_index] / (max - min) * bins) << "\t";
        }
        histogram_of << endl;
    }

}