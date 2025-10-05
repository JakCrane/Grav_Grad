#include "BaseField.h"

BaseField::BaseField(int Nx, int Ny,
             double x_min, double x_max,
             double y_min, double y_max)
    : m_Nx(Nx), m_Ny(Ny),
      m_x_min(x_min), m_x_max(x_max),
      m_y_min(y_min), m_y_max(y_max),
      m_dx((x_max - x_min) / (Nx - 1)),
      m_dy((y_max - y_min) / (Ny - 1)),
      m_domain(Nx * Ny)
{
}

BaseField::BaseField(int N, double size)
    : m_Nx(N), m_Ny(N),
      m_x_min(-size / 2), m_x_max(size / 2),
      m_y_min(-size / 2), m_y_max(size / 2),
      m_dx(size / (N - 1)),
      m_dy(size / (N - 1)),
      m_domain(N * N)
{
}

double BaseField::valueAt(const int i, const int j) const {
    return m_domain[i * m_Nx + j];
}

std::array<double,2> BaseField::gradientAt(const double x, const double y) const {

    // Find nearest grid indices
    int i = clamp(int((x - m_x_min)/m_dx), 1, m_Nx-2);
    int j = clamp(int((y - m_y_min)/m_dy), 1, m_Ny-2);

    // Finite differences
    if (i <= 0 || i >= m_Nx-1 || j <= 0 || j >= m_Ny-1) {
        return {0.0, 0.0}; // Out of bounds or on edge, return zero gradient
    }
    double dPhidx = (m_domain[(i+1) * m_Nx + j] - m_domain[(i-1) * m_Nx + j]) / (2*m_dx);
    double dPhidy = (m_domain[i * m_Nx + j + 1] - m_domain[i * m_Nx + j - 1]) / (2*m_dy);

    return {-dPhidx, -dPhidy};  // acceleration vector
}

void BaseField::exportCSV(const std::string& filename, const int frame) const {
    std::ofstream fout(filename, std::ios::app); // append mode
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            fout << frame << "," << x << "," << y << "," << m_domain[i * m_Nx + j] << "\n";
        }
    }
}

void BaseField::printInfo() const {
    std::cout << "Field Info:\n";
    std::cout << "  Grid size: " << m_Nx << " x " << m_Ny << "\n";
    std::cout << "  X range: [" << m_x_min << ", " << m_x_max << "]\n";
    std::cout << "  Y range: [" << m_y_min << ", " << m_y_max << "]\n";
    std::cout << "  dx: " << m_dx << ", dy: " << m_dy << "\n";
    std::cout << "  Domain values:\n";
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            std::cout << m_domain[i * m_Nx + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "  Position values:\n";
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            std::cout << m_domain[i * m_Nx + j] << " ";
        }
        std::cout << "\n";
    }
}