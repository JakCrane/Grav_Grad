#include "GravitationalPotentialField.h"

void GravitationalPotentialField::setup(const std::array<double, 2> mass_pos, const double G, const double M) 
{
    m_G = G;
    m_M = M;
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            m_domain(i, j) = computePotential(x, y, mass_pos);
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
            m_domain(i, j) = computePotential(x, y, mass_field);
        }
    }
}

void GravitationalPotentialField::update(const std::array<double, 2> mass_pos) 
{
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            m_domain(i, j) = computePotential(x, y, mass_pos);
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
            m_domain(i, j) = computePotential(x, y, mass_field);
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

// double GravitationalPotentialField::computePotential(const double x_of_gravitational_potential, const double y_of_gravitational_potential, const MassField& mass_field) {
//     double potential = 0.0;
//     for (int i = 0; i < m_Nx; ++i) {

//         const double x_of_mass_field = mass_field.getXCoord(i);
//         const double x_distance_squared = (x_of_gravitational_potential - x_of_mass_field)*(x_of_gravitational_potential - x_of_mass_field);

//         for (int j = 0; j < m_Ny; ++j) {

//             const double y_of_mass_field = mass_field.getYCoord(j);
//             const double y_distance_squared = (y_of_gravitational_potential - y_of_mass_field)*(y_of_gravitational_potential - y_of_mass_field);

//             const double r_of_mass_field = std::sqrt(x_distance_squared + y_distance_squared);
//             if (r_of_mass_field < 1e-6) continue;
//             const double mass_at_point = mass_field.valueAt(i, j);
//             potential += mass_at_point / r_of_mass_field; // -m_G * is done later for speed (-G*M/r)
//         }
//     }
//     return -m_G * potential; // -m_G * is done here for speed
// }

double GravitationalPotentialField::computePotential(const double x_of_gravitational_potential, const double y_of_gravitational_potential, const MassField& mass_field) {
    m_current_r_inv_field = Eigen::MatrixXd::Zero(m_Nx, m_Ny);
    for (int i = 0; i < m_Nx; ++i) {

        const double x_of_mass_field = mass_field.getXCoord(i);
        const double x_distance_squared = (x_of_gravitational_potential - x_of_mass_field)*(x_of_gravitational_potential - x_of_mass_field);
        for (int j = 0; j < m_Ny; ++j) {
            const double y_of_mass_field = mass_field.getYCoord(j);
            const double y_distance_squared = (y_of_gravitational_potential - y_of_mass_field)*(y_of_gravitational_potential - y_of_mass_field);

            const double r_of_mass_field = std::sqrt(x_distance_squared + y_distance_squared);
            m_current_r_inv_field(i,j) = r_of_mass_field + 1e-6; // avoid singularity at r=0
        }
    }
    return -m_G * (m_current_mass_field.array() / m_current_r_inv_field.array()).sum();
}
