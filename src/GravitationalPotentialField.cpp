#include "GravitationalPotentialField.h"

void GravitationalPotentialField::setup(const std::array<double, 2> mass_pos, const double G, const double M) 
{
    m_G = G;
    m_M = M;
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            m_domain[i * m_Nx + j] = computePotential(x, y, mass_pos);
        }
    }
}

void GravitationalPotentialField::setup(const MassField& mass_field, const double G, const double M) 
{
    m_G = G;
    m_M = M;
    m_current_mass_field = mass_field.getDomain();
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            const double x = getXCoord(i);
            const double y = getYCoord(j);
            m_domain[i * m_Nx + j] = computePotential(x, y, mass_field);
        }
    }
}

void GravitationalPotentialField::update(const std::array<double, 2> mass_pos) 
{
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            m_domain[i * m_Nx + j] = computePotential(x, y, mass_pos);
        }
    }
}
void GravitationalPotentialField::update(const MassField& mass_field) 
{
    m_current_mass_field = mass_field.getDomain();
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            m_domain[i * m_Nx + j] = computePotential(x, y, mass_field);
        }
    }
}


double GravitationalPotentialField::computePotential(double x_of_gravitational_potential, double y_of_gravitational_potential, const std::array<double, 2>& mass_pos) {
    const double dx = x_of_gravitational_potential - mass_pos[0];
    const double dy = y_of_gravitational_potential - mass_pos[1];
    const double r = std::sqrt(dx*dx + dy*dy);
    // std::cout << "x: " << x << ", y: " << y << ", r: " << r << ", domain: " << (r < 1e-6 ? 0.0 : -G * M / r) << std::endl;
    if (r < 1e-6) return 0.0;
    return -m_G * m_M / r;
}

double GravitationalPotentialField::computePotential(
    const double x_of_gravitational_potential,
    const double y_of_gravitational_potential,
    const MassField& mass_field)
{
    double sum = 0.0;

    for (int i = 0; i < m_Nx; ++i) {
        const double x_of_mass_field = mass_field.getXCoord(i);
        const double x_distance_squared = (x_of_gravitational_potential - x_of_mass_field) * (x_of_gravitational_potential - x_of_mass_field);

        for (int j = 0; j < m_Ny; ++j) {
            const double y_of_mass_field = mass_field.getYCoord(j);
            const double y_distance_squared = (y_of_gravitational_potential - y_of_mass_field) * (y_of_gravitational_potential - y_of_mass_field);

            const double r_of_mass_field = std::sqrt(x_distance_squared + y_distance_squared);

            sum += m_current_mass_field[i * m_Nx + j] / (r_of_mass_field + 1e-6);
        }
    }

    return -m_G * sum;
}
