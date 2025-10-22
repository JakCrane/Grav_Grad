#include "ThreeD/GravitationalAccelerationField.h"

template <bool UsePotential>
GravitationalAccelerationField<UsePotential>::GravitationalAccelerationField(int N, 
                    double size, 
                    const MassField& mass_field,
                    double G) 
                    : VectorField(N, size),
                      m_mass_field(&mass_field),
                      m_G(G),
                      m_potential_field(UsePotential ? GravitationalPotentialField(N, size, &mass_field, G)
                                        : GravitationalPotentialField{})
{
    update();
}

template <bool UsePotential>
std::array<double, 3> GravitationalAccelerationField<UsePotential>::computeAcceleration(double x, double y, double z)
{
    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_z = 0.0;
    const auto& mass_field_values = m_mass_field->getDomain();
    
    const double dx_step = (m_mass_field->getXCoord(1) - m_mass_field->getXCoord(0));
    const double dy_step = (m_mass_field->getYCoord(1) - m_mass_field->getYCoord(0));
    const double dz_step = (m_mass_field->getZCoord(1) - m_mass_field->getZCoord(0));

    for (int i = 0; i < m_Nx; ++i) {
        double local_sum_x = 0.0, local_sum_y = 0.0, local_sum_z = 0.0;
        const double dx = x - (m_x_min + i * dx_step);
        for (int j = 0; j < m_Ny; ++j) {
            const double dy = y - (m_y_min + j * dy_step);
            for (int k = 0; k < m_Nz; ++k) {
                const double dz = z - (m_z_min + k * dz_step);
                const double r2 = dx*dx + dy*dy + dz*dz;
                if (r2 < 1e-6) continue;
                
                const double inv_r = 1.0 / std::sqrt(r2);
                const double inv_r3 = inv_r * inv_r * inv_r;
                const double mass = mass_field_values[i * m_Ny * m_Nz + j * m_Nz + k];
                local_sum_x += mass * dx * inv_r3;
                local_sum_y += mass * dy * inv_r3;
                local_sum_z += mass * dz * inv_r3;
            }
        }
        sum_x += local_sum_x;
        sum_y += local_sum_y;
        sum_z += local_sum_z;
    }

    return {m_G * sum_x, m_G * sum_y, m_G * sum_z};
}

template <>
void GravitationalAccelerationField<false>::update() 
{
    #pragma omp parallel for collapse(3) schedule(static)
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            for (int k = 0; k < m_Nz; ++k) {
                const double x = getXCoord(i);
                const double y = getYCoord(j);
                const double z = getZCoord(k);
                m_domain[i * m_Ny + j] = computeAcceleration(x, y, z);
            }
        }
    }
}

template <>
void GravitationalAccelerationField<true>::update()
{
    m_potential_field.update();

    const double dx = m_dx;
    const double dy = m_dy;
    const double dz = m_dz;

    // Parallel central difference
    #pragma omp parallel for collapse(3) schedule(static)
    for (int i = 1; i < m_Nx - 1; ++i) {
        for (int j = 1; j < m_Ny - 1; ++j) {
            for (int k = 1; k < m_Nz - 1; ++k) {
                const int idx = i * m_Ny * m_Nz + j * m_Nz + k;

                // NOTE: include k in the x/y stencils (bug in your snippet)
                const double phi_ip1 = m_potential_field.valueAt(i+1, j,   k);
                const double phi_im1 = m_potential_field.valueAt(i-1, j,   k);
                const double phi_jp1 = m_potential_field.valueAt(i,   j+1, k);
                const double phi_jm1 = m_potential_field.valueAt(i,   j-1, k);
                const double phi_kp1 = m_potential_field.valueAt(i,   j,   k+1);
                const double phi_km1 = m_potential_field.valueAt(i,   j,   k-1);

                const double dphidx = (phi_ip1 - phi_im1) / (2.0 * dx);
                const double dphidy = (phi_jp1 - phi_jm1) / (2.0 * dy);
                const double dphidz = (phi_kp1 - phi_km1) / (2.0 * dz);

                m_domain[idx][0] = -dphidx;  // g_x
                m_domain[idx][1] = -dphidy;  // g_y
                m_domain[idx][2] = -dphidz;  // g_z
            }
        }
    }


    auto idx = [&](int i, int j, int k) {
        return i * m_Ny * m_Nz + j * m_Nz + k;
    };

    // --- X boundaries ---
    for (int j = 0; j < m_Ny; ++j) {
        for (int k = 0; k < m_Nz; ++k) {
            m_domain[idx(0,         j, k)] = m_domain[idx(1,         j, k)];
            m_domain[idx(m_Nx - 1,  j, k)] = m_domain[idx(m_Nx - 2,  j, k)];
        }
    }

    // --- Y boundaries ---
    for (int i = 0; i < m_Nx; ++i) {
        for (int k = 0; k < m_Nz; ++k) {
            m_domain[idx(i, 0,         k)] = m_domain[idx(i, 1,         k)];
            m_domain[idx(i, m_Ny - 1,  k)] = m_domain[idx(i, m_Ny - 2,  k)];
        }
    }

    // --- Z boundaries ---
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            m_domain[idx(i, j, 0       )] = m_domain[idx(i, j, 1       )];
            m_domain[idx(i, j, m_Nz - 1)] = m_domain[idx(i, j, m_Nz - 2)];
        }
    }

}

// Explicit instantiation for both variants
template class GravitationalAccelerationField<true>;
template class GravitationalAccelerationField<false>;