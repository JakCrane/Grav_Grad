#include "MassField.h"
#include <iomanip>

MassField::MassField(int N, 
                    double size, 
                    double side_length_x, 
                    double side_length_y,
                    double density, 
                    std::array<double, 2> initial_position)  
                    : ScalarField(N, size),
                      m_density(density),
                      m_side_length_x(side_length_x),
                      m_side_length_y(side_length_y),
                      m_type(FieldType::RECTANGULAR)
{
    update(initial_position);
}

MassField::MassField(int N, 
                    double size, 
                    double radius,
                    double density, 
                    std::array<double, 2> initial_position)  
                    : ScalarField(N, size),
                      m_density(density),
                      m_radius(radius),
                      m_type(FieldType::CIRCULAR)
{
    update(initial_position);
}

void MassField::update(std::array<double, 2> mass_pos) 
{
    m_current_position = mass_pos;
    clearField();
    if (m_type == FieldType::RECTANGULAR) {
        fillRectangularRegion();
    } else if (m_type == FieldType::CIRCULAR) {
        fillCircularRegion();
    }
}

void MassField::clearField() 
{
    std::fill(m_domain.begin(), m_domain.end(), 0.0);
}

void MassField::fillRectangularRegion() 
{
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            double x = m_x_min + i * (m_x_max - m_x_min) / (m_Nx - 1);
            double y = m_y_min + j * (m_y_max - m_y_min) / (m_Ny - 1);
            if (std::abs(x - m_current_position[0]) < m_side_length_x / 2 &&
                std::abs(y - m_current_position[1]) < m_side_length_y / 2) {
                m_domain[i * m_Ny + j] = m_density * m_dx * m_dy;
            }
        }
    }
}

void MassField::fillCircularRegion()
{
    const double edge_smooth = 0.00 * m_radius; // Smoothing width at the edge
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            double x = m_x_min + i * (m_x_max - m_x_min) / (m_Nx - 1);
            double y = m_y_min + j * (m_y_max - m_y_min) / (m_Ny - 1);

            double dx = x - m_current_position[0];
            double dy = y - m_current_position[1];
            double r = std::sqrt(dx*dx + dy*dy);

            // Smooth transition over width 'edge_smooth'
            double transition = 0.5 * (1.0 - std::tanh((r - m_radius) / edge_smooth));

            // transition ≈ 1 inside, ≈ 0 outside
            m_domain[i * m_Ny + j] = m_density * m_dx * m_dy * transition;
        }
    }
}