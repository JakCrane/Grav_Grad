#include "TwoD/GravitationalPotentialField.h"
#include <iomanip>

GravitationalPotentialField::GravitationalPotentialField(int N, 
                    double size, 
                    const MassField* mass_field, 
                    double G)  
                    : ScalarField(N, size),
                      m_mass_field(mass_field),
                      m_G(G)
{
    // build coordinate arrays
    xs.resize(m_Nx);
    ys.resize(m_Ny);
    for (int i = 0; i < m_Nx; ++i) xs[i] = getXCoord(i);
    for (int j = 0; j < m_Ny; ++j) ys[j] = getYCoord(j);
}

void GravitationalPotentialField::update() 
{
    const int Nx = m_Nx, Ny = m_Ny;
    const double G = m_G;
    const double eps2 = (m_dx * m_dx) + (m_dy * m_dy);

    const auto& mass_field_values = m_mass_field->getDomain();
    const double* __restrict__ mass = mass_field_values.data();
    double* __restrict__ phi = m_domain.data();

    // Parallel across target grid; inner loops vectorize across j2
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < Nx; ++i) {
        for (int j = 0; j < Ny; ++j) {
            const double x = xs[i];
            const double y = ys[j];

            double pot = 0.0;

            for (int i2 = 0; i2 < Nx; ++i2) {
                const double dx  = x - xs[i2];
                const double dx2 = dx * dx;

                const double* __restrict__ mass_row = mass + i2 * Ny;
                // Vectorize across contiguous j2
                #pragma omp simd reduction(+:pot)
                for (int j2 = 0; j2 < Ny; ++j2) {
                    const double dy  = y - ys[j2];
                    const double r2  = dx2 + dy * dy + eps2; // branchless
                    pot -= mass_row[j2] * (1.0 / std::sqrt(r2));
                }
            }
            phi[i * Ny + j] = G * pot;
        }
    }
}