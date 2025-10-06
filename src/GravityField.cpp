#include "GravityField.h"

GravityField::GravityField(int N, 
                    double size, 
                    const MassField& mass_field,
                    double G) 
                    : TwoDVectorField(N, size),
                      m_mass_field(&mass_field),
                      m_G(G)
{
    update();
}


void GravityField::update() 
{
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            m_domain[i * m_Ny + j] = computeAcceleration(x, y);
        }
    }
}

std::array<double, 2> GravityField::computeAcceleration(const double x, const double y)
{
    double sum_x = 0.0;
    double sum_y = 0.0;

    #pragma omp parallel for collapse(2) reduction(+:sum_x,sum_y) schedule(static)
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            const double dx = x - m_mass_field->getXCoord(i);
            const double dy = y - m_mass_field->getYCoord(j);
            const double r_squared = dx*dx + dy*dy;
            if (r_squared < 1e-6) continue; // Skip singularity
            const double r = std::sqrt(r_squared);
            const double denominator = 1.0 / (r_squared * r); // r^3
            const double mass = m_mass_field->valueAt(i, j);
            sum_x += mass * dx * denominator;
            sum_y += mass * dy * denominator;
        }
    }

    return {m_G * sum_x, m_G * sum_y};
}
