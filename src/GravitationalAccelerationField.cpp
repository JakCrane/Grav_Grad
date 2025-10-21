#include "GravitationalAccelerationField.h"

template <bool UsePotential>
GravitationalAccelerationField<UsePotential>::GravitationalAccelerationField(int N, 
                    double size, 
                    const MassField& mass_field,
                    double G) 
                    : TwoDVectorField(N, size),
                      m_mass_field(&mass_field),
                      m_G(G),
                      m_potential_field(UsePotential ? GravitationalPotentialField(N, size, &mass_field, G)
                                        : GravitationalPotentialField{})
{
    update();
}

template <bool UsePotential>
std::array<double, 2> GravitationalAccelerationField<UsePotential>::computeAcceleration(double x, double y)
{
    double sum_x = 0.0;
    double sum_y = 0.0;
    const auto& mass_field_values = m_mass_field->getDomain();
    
    const double dx_step = (m_mass_field->getXCoord(1) - m_mass_field->getXCoord(0));
    const double dy_step = (m_mass_field->getYCoord(1) - m_mass_field->getYCoord(0));
    
    for (int i = 0; i < m_Nx; ++i) {
        double local_sum_x = 0.0, local_sum_y = 0.0;
        const double dx = x - (m_x_min + i * dx_step);
        for (int j = 0; j < m_Ny; ++j) {
            const double dy = y - (m_y_min + j * dy_step);
            const double r2 = dx*dx + dy*dy;
            if (r2 < 1e-6) continue;
            
            const double inv_r = 1.0 / std::sqrt(r2);
            const double inv_r3 = inv_r * inv_r * inv_r;
            const double mass = mass_field_values[i * m_Ny + j];
            local_sum_x += mass * dx * inv_r3;
            local_sum_y += mass * dy * inv_r3;
        }
        sum_x += local_sum_x;
        sum_y += local_sum_y;
    }

    return {m_G * sum_x, m_G * sum_y};
}

template <>
void GravitationalAccelerationField<false>::update() 
{
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            const double x = getXCoord(i);
            const double y = getYCoord(j);
            m_domain[i * m_Ny + j] = computeAcceleration(x, y);
        }
    }
}

template <>
void GravitationalAccelerationField<true>::update()
{
    const double dx = (m_x_max - m_x_min) / (m_Nx - 1);
    const double dy = (m_y_max - m_y_min) / (m_Ny - 1);

    // Parallel central difference
    #pragma omp parallel for collapse(2)
    for (int i = 1; i < m_Nx - 1; ++i) {
        for (int j = 1; j < m_Ny - 1; ++j) {
            const int idx = i * m_Ny + j;

            // finite differences of potential
            const double dphidx = (m_potential_field.valueAt(i + 1, j) - m_potential_field.valueAt(i - 1, j)) / (2.0 * dx);
            const double dphidy = (m_potential_field.valueAt(i, j + 1) - m_potential_field.valueAt(i, j - 1)) / (2.0 * dy);

            // store acceleration (negative gradient)
            m_domain[idx][0] = -dphidx;  // g_x
            m_domain[idx][1] = -dphidy;  // g_y
        }
    }

    // Optional: handle boundaries by copying neighboring values
    for (int i = 0; i < m_Nx; ++i) {
        m_domain[i * m_Ny + 0]          = m_domain[i * m_Ny + 1];
        m_domain[i * m_Ny + (m_Ny - 1)] = m_domain[i * m_Ny + (m_Ny - 2)];
    }
    for (int j = 0; j < m_Ny; ++j) {
        m_domain[0 * m_Ny + j]          = m_domain[1 * m_Ny + j];
        m_domain[(m_Nx - 1) * m_Ny + j] = m_domain[(m_Nx - 2) * m_Ny + j];
    }
}

// Explicit instantiation for both variants
template class GravitationalAccelerationField<true>;
template class GravitationalAccelerationField<false>;