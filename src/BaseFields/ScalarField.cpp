#include "BaseFields/ScalarField.h"


std::array<double,2> ScalarField::gradientAt(const double x, const double y) const {

    // Find nearest grid indices
    int i = clamp(int((x - m_x_min)/m_dx), 1, m_Nx-2);
    int j = clamp(int((y - m_y_min)/m_dy), 1, m_Ny-2);

    // Finite differences
    if (i <= 0 || i >= m_Nx-1 || j <= 0 || j >= m_Ny-1) {
        return {0.0, 0.0}; // Out of bounds or on edge, return zero gradient
    }
    double dPhidx = (m_domain[(i+1) * m_Ny + j] - m_domain[(i-1) * m_Ny + j]) / (2*m_dx);
    double dPhidy = (m_domain[i * m_Ny + j + 1] - m_domain[i * m_Ny + j - 1]) / (2*m_dy);

    return {-dPhidx, -dPhidy};  // acceleration vector
}

void ScalarField::doExportCSV(const std::string& filename, const int frame) const {
    std::ofstream fout(filename, std::ios::app); // append mode
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            fout << frame << "," << x << "," << y << "," << m_domain[i * m_Ny + j] << "\n";
        }
    }
}