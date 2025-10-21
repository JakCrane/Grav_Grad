#include "GravitationalPotentialField.h"
#include <iomanip>

GravitationalPotentialField::GravitationalPotentialField(int N, 
                    double size, 
                    const MassField* mass_field, 
                    double G)  
                    : ScalarField(N, size),
                      m_mass_field(mass_field),
                      m_G(G)
{
}

void GravitationalPotentialField::update() 
{
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            const double x = getXCoord(i);
            const double y = getYCoord(j);
            m_domain[i * m_Ny + j] = computePotential(x, y);
        }
    }
}

double GravitationalPotentialField::computePotential(double x, double y)
{
    double potential = 0.0;
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            const double dx = x - m_mass_field->getXCoord(i); // r - r_ij
            const double dy = y - m_mass_field->getYCoord(j); // r - r_ij
            const double r_squared = dx*dx + dy*dy;
            if (r_squared < 1e-6) continue;
            const double r = std::sqrt(r_squared);
            const double mass = m_mass_field->getDomain()[i * m_Ny + j];
            potential -= m_G * mass / r;
        }
    }
    return potential;
}