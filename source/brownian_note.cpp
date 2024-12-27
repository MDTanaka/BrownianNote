#pragma once

#include "brownian_note.h"

namespace BrownNotes
{
    void BrownianNote::initialize(double sample_rate)
    {
        this->sample_rate = sample_rate;
        max_freq = sample_rate / 2;

        f.reserve(n_particles);
        phi.reserve(n_particles);
        for (size_t p = 0; p < n_particles; p++)
        {
            f.push_back(inv_harmonic_cdf(random_uniform(e2), min_freq, max_freq));
            phi.push_back(PI2 * random_uniform(e2));
        }
    }

    void BrownianNote::handle_note_on(double freq)
    {
        comb_freq = freq;
        comb_depth = comb_max_depth;
    }

    void BrownianNote::handle_note_off()
    {
        comb_depth = 0;
    }

    void BrownianNote::process(int n_samples, float *out)
    {
        double D = get_effective_diffusion_const();
        int batchsize = get_batchsize();

        double dx;
        double x_pred;
        double pi2dt = PI2 / sample_rate;
        double volume = 4.0 / n_particles;

        int batch_index = 0;
        for (size_t sample = 0; sample < n_samples; sample++)
        {
            // UPDATE PARTICLE FREQUENCIES
            batch_index = sample % n_batches;
            for (size_t particle = batchsize * batch_index; particle < batchsize * (batch_index + 1); particle++)
            {
                // Standing distribution + comb force and browinan motion
                dx = D * (-1 / f[particle] + comb_depth * periodic_harmonic_force(f[particle], comb_freq)) + std::sqrt(2 * D) * random_normal(e2);
                // Reflect at edges
                x_pred = f[particle] + dx;
                if ((x_pred < min_freq) || (x_pred >= max_freq))
                    dx *= -1;
                f[particle] += dx;
            }
            // WRITE OUTPUT
            out[sample] = 0;
            for (size_t particle = 0; particle < n_particles; particle++)
            {
                phi[particle] += f[particle] * pi2dt;
                out[sample] += volume * std::sin(phi[particle]);
            }
        }
    }

    double inv_harmonic_cdf(double x, double a, double b)
    {
        return a * std::exp(-x * std::log(a / b));
    }

    double periodic_harmonic_force(double x, double L)
    {
        if (x <= L / 2)
            return -2 * (2 * x / L - 2);
        else
            return -2 * (std::fmod(2 * x / L - 1, 2.0) - 1);
    }
}
