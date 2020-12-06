#include <bits/stdc++.h>

using namespace std;
using namespace literals::string_literals;

struct NormalDistSampler {
    uniform_real_distribution<double> uniform_dist;
    mt19937 rng;
    bool has_z2;
    double z2;
    unsigned long long uniform_call;
    NormalDistSampler () {
        random_device seed_gen;
        rng = mt19937(seed_gen());
        uniform_dist = uniform_real_distribution<double>();
        has_z2 = false;
        uniform_call = 0;
    }
    const double uniform () {
        uniform_call++;
        return uniform_dist(rng);
    }
    virtual const double sample () = 0;
    virtual const string name () = 0; 
};

struct CLT : NormalDistSampler {
    const double sample () {
        double sum = -6;
        for (int i = 0; i < 12; i++) {
            sum += uniform();
        }
        return sum;
    }
    const string name () {
        return "CLT による近似"s;
    }
};

struct BoxMuller : NormalDistSampler {
    double tau;
    BoxMuller () {
        tau = 2 * numbers::pi;
    }
    const double sample () {
        if (has_z2) {
            has_z2 = false;
            return z2;
        }
        double u1 = uniform();
        double u2 = uniform();
        double R = sqrt(-2 * log(u1));
        z2 = R * sin(tau * u2);
        has_z2 = true;
        return R * cos(tau * u2);
    }
    const string name () {
        return "Box-Muller 法"s;
    }
};

struct Polar : NormalDistSampler {
    const double sample () {
        if (has_z2) {
            has_z2 = false;
            return z2;
        }
        double u1, u2, s;
        do {
            u1 = uniform() * 2 - 1;
            u2 = uniform() * 2 - 1;
            s = u1 * u1 + u2 * u2;
        } while (s < 0 || 1 < s);
        double R = sqrt(-2 * log(s) / s);
        z2 = u2 * R;
        has_z2 = true;
        return u1 * R;
    }
    const string name () {
        return "Polar 法"s;
    }
};

struct Kinderman : NormalDistSampler {
    double BX, BY, LOG2PI;
    Kinderman () {
        BX = 1 / sqrt(sqrt(2 * numbers::pi));
        BY = sqrt(sqrt(2 / numbers::pi) / numbers::e);
        LOG2PI = log(2 * numbers::pi);
    }
    const double sample () {
        double u1, u2, z;
        do {
            u1 = uniform() * BX;
            u2 = uniform() * 2 * BY - BY;
            z = u2 / u1;
        } while (-4 * log(u1) < z * z + LOG2PI);
        return z;
    }
    const string name () {
        return "Kinderman 法"s;
    }
};

struct MontyPython : NormalDistSampler {
    double a, b, s, p, q, d;
    MontyPython () {
        a = sqrt(log(4));
        b = sqrt(2 * numbers::pi);
        s = a / (b - a);
        p = (s + 1) / 2;
        q = log(s);
        d = b * b;
    }
    const pair<int, double> signed_uniform () {
        double u = uniform() * 2;
        if (u <= 1) {
            return make_pair(1, u);
        } else {
            return make_pair(-1, u - 1);
        }
    }
    const double sample () {
        const auto [sign, u1] = signed_uniform();
        const double ux = u1 * b;
        if (ux < a) {
            return copysign(ux, sign);
        }
        const double u2 = uniform() / 2;
        if (log(u2) < -(ux * ux / 2)) {
            return copysign(ux, sign);
        }
        const double Y = copysign(s * (b - ux), sign);
        if (log(p - u2) < q - Y * Y / 2) {
            return Y;
        }
        double u3, u4, X;
        do {
            u3 = uniform();
            u4 = uniform();
            X = sqrt(d - 2 * log(1 - u3));
        } while (b < X * u4);
        return copysign(X, sign);
    }
    const string name () {
        return "Monty-Python 法"s;
    }
};

template<size_t k = 8, typename ki_type = uint64_t>
struct ZigguratLUT {
    static_assert(
        is_same<ki_type, uint64_t>::value || is_same<ki_type, uint32_t>::value,
        "ki_type must be either uint64_t or uint32_t"
    );
    static_assert(
        k == 7 || k == 8,
        "k must be either 7 or 8"
    );
    const double r = k == 7 ? 3.442619855899 : 3.6541528853610088;
    const double v = k == 7 ? 0.00991256303526217 : 0.00492867323399;
    const size_t m = 8 * sizeof(ki_type);
    const size_t n = 1 << k;
    array<double, (1 << k)> W, F;
    array<ki_type, (1 << k)> K;
    ZigguratLUT () {
        W[n - 1] = v * exp(0.5 * r * r) / ((ki_type)1 << (m - k - 1));
        W[n - 2] = r / ((ki_type)1 << (m - k - 1));
        K[n - 1] = floor(r / W[n - 1]);
        F[n - 1] = exp(-0.5 * r * r);
        double x = r;
        for (size_t i = n - 2; i >= 1; i--) {
            x = sqrt(-2 * log(exp(-0.5 * x * x) + v / x));
            W[i - 1] = x / ((ki_type)1 << (m - k - 1));
            K[i] = floor(x / W[i]);
            F[i] = exp(-0.5 * x * x);
        }
        K[0] = 0;
        F[0] = 1;
    }
};

template<size_t k = 8, typename mbit_uint = uint64_t>
struct ZigguratAlgorithm : NormalDistSampler {
    static_assert(
        is_same<mbit_uint, uint64_t>::value || is_same<mbit_uint, uint32_t>::value,
        "mbit_uint must be either uint64_t or uint32_t"
    );
    static_assert(
        k == 7 || k == 8,
        "k must be either 7 or 8"
    );
    ZigguratLUT<k, mbit_uint> lut;
    uniform_int_distribution<mbit_uint> uniform_uint_dist;
    const size_t n = 1 << k;
    const mbit_uint uniform_uint () {
        uniform_call++;
        return uniform_uint_dist(rng);
    }
    const double sample_from_tail (double b) {
        double d = b * b, u1, u2, X;
        do {
            u1 = uniform();
            u2 = uniform();
            X = sqrt(d - 2 * log(1 - u1));
        } while (b < X * u2);
        return X;
    }
    const double sample () {
        mbit_uint ui = uniform_uint();
        mbit_uint i = ui & (((mbit_uint)1 << k) - 1);
        ui = ui >> k;
        int sign = ((ui & 1) == 0) ? 1 : -1;
        ui = ui >> 1;
        if (ui < lut.K[i]) {
            return copysign(ui * lut.W[i], sign);
        }
        if (i == n - 1) {
            return copysign(sample_from_tail(lut.r), sign);
        } else {
            double ux = ui * lut.W[i];
            double f = exp(-0.5 * ux * ux);
            double u = uniform();
            if (u * (lut.F[i] - lut.F[i + 1]) < f - lut.F[i + 1]) {
                return copysign(ux, sign);
            } else {
                return sample();
            }
        }
    }
    const string name () {
        return "Ziggurat 法"s;
    }
};

int main (int argc, char* argv[]) {

    // $ ./main {number of samples} {0=benchmark only, 1=output generated numbers}
    const size_t n = stoi(argv[1]);
    const bool output = stoi(argv[2]);

    CLT cl;
    BoxMuller bm;
    Polar po;
    Kinderman km;
    MontyPython mp;
    ZigguratAlgorithm<8, uint32_t> za;

    vector<NormalDistSampler*> samplers = {&cl, &bm, &po, &km, &mp, &za};

    vector<vector<double>> results(samplers.size());
    chrono::system_clock::time_point start, end;

    ofstream result_of, benchmark_of;
    
    if (output) {
        result_of = ofstream("result.txt", ios_base::out);
        result_of << setprecision(8) << fixed;
    } else {
        benchmark_of = ofstream("benchmark.txt", ios_base::out);
        benchmark_of << setprecision(4) << fixed;
        benchmark_of << n << "\n";
        benchmark_of << "algorithm" << "\t";
        benchmark_of << "time" << "\t";
        benchmark_of << "rate" << "\t";
        benchmark_of << "samples" << endl;
    }

    for (size_t sampler_index = 0; sampler_index < samplers.size(); sampler_index++) {
        if (output) {
            results[sampler_index].resize(n);
        }
        start = chrono::system_clock::now();
        for (size_t i = 0; i < n; i++) {
            if (output) {
                results[sampler_index][i] = samplers[sampler_index]->sample();
            } else {
                samplers[sampler_index]->sample();
            }
        }
        end = chrono::system_clock::now();
        double time = chrono::duration_cast<chrono::microseconds>(end - start).count();
        if (!output) {
            benchmark_of << samplers[sampler_index]->name() << "\t";
            benchmark_of << (time) << "\t";
            benchmark_of << (n / time * 1000000) << "\t";
            benchmark_of << ((samplers[sampler_index]->uniform_call) / (double)n) << endl;
        }
    }

    if (output) {
        result_of << n << "\t" << samplers.size() << endl;
        for (size_t i = 0; i < n; i++) {
            for (size_t sampler_index = 0; sampler_index < samplers.size(); sampler_index++) {
                result_of << results[sampler_index][i] << "\t";
            }
            result_of << endl;
        }
    }

}