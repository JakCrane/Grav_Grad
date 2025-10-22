#include "ThreeD/GravitationalPotentialField.h"
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
    zs.resize(m_Nz);
    for (int i = 0; i < m_Nx; ++i) xs[i] = getXCoord(i);
    for (int j = 0; j < m_Ny; ++j) ys[j] = getYCoord(j);
    for (int k = 0; k < m_Nz; ++k) zs[k] = getZCoord(k);
}

void GravitationalPotentialField::update() 
{
    const int Nx = m_Nx, Ny = m_Ny, Nz = m_Nz;
    const double G = m_G;
    const double eps2 = (m_dx * m_dx) + (m_dy * m_dy) + (m_dz * m_dz);

    const auto& mass_field_values = m_mass_field->getDomain();
    const double* __restrict__ mass = mass_field_values.data();
    double* __restrict__ phi = m_domain.data();

    // Precomputed coordinate arrays (from constructor)
    const double* __restrict__ X = xs.data();
    const double* __restrict__ Y = ys.data();
    const double* __restrict__ Z = zs.data(); // you'll need to add zs[] like xs/ys

    // Triple-parallel across target grid (each cell in potential field)
    #pragma omp parallel for collapse(3) schedule(static)
    for (int i = 0; i < Nx; ++i) {
        for (int j = 0; j < Ny; ++j) {
            for (int k = 0; k < Nz; ++k) {
                const double x = X[i];
                const double y = Y[j];
                const double z = Z[k];

                double pot = 0.0;

                // Sum over all mass cells
                for (int i2 = 0; i2 < Nx; ++i2) {
                    const double dx  = x - X[i2];
                    const double dx2 = dx * dx;

                    for (int j2 = 0; j2 < Ny; ++j2) {
                        const double dy  = y - Y[j2];
                        const double dy2 = dy * dy;

                        const double* __restrict__ mass_line =
                            mass + (i2 * Ny + j2) * Nz;

                        // Vectorize over contiguous z2
                        #pragma omp simd reduction(+:pot)
                        for (int k2 = 0; k2 < Nz; ++k2) {
                            const double dz  = z - Z[k2];
                            const double r2  = dx2 + dy2 + dz * dz + eps2;
                            pot -= mass_line[k2] * (1.0 / std::sqrt(r2));
                        }
                    }
                }

                phi[(i * Ny + j) * Nz + k] = G * pot;
            }
        }
    }
}
