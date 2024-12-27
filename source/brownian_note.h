#pragma once

#include <random>

#define PI2 2 * 3.14159265358979323846

#define N_PARTICLES 500
#define MIN_FREQ 100
#define N_BATCHES 100
#define DIFFUSION_CONSTANT 0.05
#define COMB_MAX_DEPTH 0.5

namespace BrownNotes
{
    class BrownianNote
    {
    public:
        BrownianNote()
            : sample_rate(44100),
              n_particles(N_PARTICLES),
              min_freq(MIN_FREQ),
              max_freq(22050),
              n_batches(N_BATCHES),
              diffusion_const(DIFFUSION_CONSTANT),
              comb_max_depth(COMB_MAX_DEPTH),
              // Start with no amplitude
              comb_depth(0),
              comb_freq(440),
              // Random stuff
              e2(std::mt19937()),
              random_uniform(0.0, 1.0),
              random_normal(0.0, 1.0)
        {
        }

        ~BrownianNote() {};

        int get_batchsize() { return n_particles / n_batches; }
        double get_effective_diffusion_const() { return diffusion_const * n_batches; }

        void initialize(double sample_rate);

        void handle_note_on(double freq);

        void handle_note_off();

        void process(int n_samples, float *out);

    protected:
        double sample_rate;
        int n_particles;
        double min_freq;
        double max_freq;
        int n_batches;
        double diffusion_const;
        double comb_max_depth;

        double comb_depth;
        double comb_freq;

        std::mt19937 e2;
        std::uniform_real_distribution<double> random_uniform;
        std::normal_distribution<double> random_normal;

        std::vector<double> f;
        std::vector<double> phi;

        int sample_count = 0;
    };

    double inv_harmonic_cdf(double x, double a, double b);

    double periodic_harmonic_force(double x, double L);
}
