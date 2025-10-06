#include "MassField.h"
#include <iomanip>

MassField::MassField(int N, 
                    double size, 
                    double side_length, 
                    double density, 
                    std::array<double, 2> initial_position)  
                    : ScalarField(N, size),
                      m_density(density),
                      m_side_length(side_length)
{
    update(initial_position);
}

void MassField::update(std::array<double, 2> mass_pos) 
{
    m_current_position = mass_pos;
    clearField();
    fillSquareRegion();
}

void MassField::clearField() 
{
    std::fill(m_domain.begin(), m_domain.end(), 0.0);
}

void MassField::fillSquareRegion() 
{
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            double x = m_x_min + i * (m_x_max - m_x_min) / (m_Nx - 1);
            double y = m_y_min + j * (m_y_max - m_y_min) / (m_Ny - 1);
            if (std::abs(x - m_current_position[0]) < m_side_length / 2 &&
                std::abs(y - m_current_position[1]) < m_side_length / 2) {
                m_domain[i * m_Ny + j] = m_density;
            }
        }
    }
}

void MassField::printDomain() const 
{
    std::cout << "SquareMassField Domain (" << m_Nx << "x" << m_Ny << "):" << std::endl;
    std::cout << "Grid bounds: x=[" << m_x_min << ", " << m_x_max << "], y=[" << m_y_min << ", " << m_y_max << "]" << std::endl;
    std::cout << "Current position: [" << m_current_position[0] << ", " << m_current_position[1] << "]" << std::endl;
    std::cout << "Density: " << m_density << ", Side length: " << m_side_length << std::endl;
    std::cout << "Domain matrix:" << std::endl;
    
    // Print the domain matrix with some formatting
    for (int j = m_Ny - 1; j >= 0; --j) {  // Print from top to bottom (higher y values first)
        for (int i = 0; i < m_Nx; ++i) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(2) << m_domain[i * m_Ny + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

