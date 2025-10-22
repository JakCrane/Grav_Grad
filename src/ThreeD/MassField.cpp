#include "ThreeD/MassField.h"
#include <iomanip>

MassField::MassField(int N, 
                    double size, 
                    double side_length_x, 
                    double side_length_y,
                    double side_length_z,
                    double density, 
                    std::array<double, 3> initial_position)  
                    : ScalarField(N, size),
                      m_density(density),
                      m_side_length_x(side_length_x),
                      m_side_length_y(side_length_y),
                      m_side_length_z(side_length_z),
                      m_type(FieldType::RECTANGULAR)
{
    update(initial_position);
}

MassField::MassField(int N, 
                    double size, 
                    double radius,
                    double density, 
                    std::array<double, 3> initial_position)  
                    : ScalarField(N, size),
                      m_density(density),
                      m_radius(radius),
                      m_type(FieldType::CIRCULAR)
{
    update(initial_position);
}

void MassField::update(std::array<double, 3> mass_pos) 
{
    m_current_position = mass_pos;
    clearField();
    if (m_type == FieldType::RECTANGULAR) {
        fillCuboidRegion();
    } else if (m_type == FieldType::CIRCULAR) {
        fillSphericalRegion();
    }
}

void MassField::clearField() 
{
    std::fill(m_domain.begin(), m_domain.end(), 0.0);
}

void MassField::fillCuboidRegion() 
{
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            for (int k = 0; k < m_Nz; ++k) {
                double x = m_x_min + i * m_dx;
                double y = m_y_min + j * m_dy;
                double z = m_z_min + k * m_dz;
                if (std::abs(x - m_current_position[0]) < m_side_length_x / 2 &&
                    std::abs(y - m_current_position[1]) < m_side_length_y / 2 &&
                    std::abs(z - m_current_position[2]) < m_side_length_z / 2) {
                    m_domain[i * m_Ny * m_Nz + j * m_Nz + k] = m_density * m_dx * m_dy * m_dz;
                }
            }
        }
    }
}

void MassField::fillSphericalRegion()
{
    const double edge_smooth = 0.01 * m_radius; // Smoothing width at the edge
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            for (int k = 0; k < m_Nz; ++k) {
                double x = m_x_min + i * m_dx;
                double y = m_y_min + j * m_dy;
                double z = m_z_min + k * m_dz;

                double dx = x - m_current_position[0];
                double dy = y - m_current_position[1];
                double dz = z - m_current_position[2];
                double r = std::sqrt(dx*dx + dy*dy + dz*dz);
                if (r > m_radius) continue;
                // Smooth transition over width 'edge_smooth'
                double transition = 0.5 * (1.0 - std::tanh((r - m_radius) / edge_smooth));

                // transition ≈ 1 inside, ≈ 0 outside
                m_domain[i * m_Ny * m_Nz + j * m_Nz + k] = m_density * m_dx * m_dy * m_dz * transition;
            }
        }
    }
}