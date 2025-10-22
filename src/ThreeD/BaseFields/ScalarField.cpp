#include "ThreeD/BaseFields/ScalarField.h"


std::array<double,3> ScalarField::gradientAt(const double x, const double y, const double z) const {

    // Find nearest grid indices
    int i = clamp(int((x - m_x_min)/m_dx), 1, m_Nx-2);
    int j = clamp(int((y - m_y_min)/m_dy), 1, m_Ny-2);
    int k = clamp(int((z - m_z_min)/m_dz), 1, m_Nz-2);

    // Finite differences
    if (i <= 0 || i >= m_Nx-1 || j <= 0 || j >= m_Ny-1 || k <= 0 || k >= m_Nz-1) {
        return {0.0, 0.0, 0.0}; // Out of bounds or on edge, return zero gradient
    }
    double dPhidx = (m_domain[(i+1) * m_Ny + j] - m_domain[(i-1) * m_Ny + j]) / (2*m_dx);
    double dPhidy = (m_domain[i * m_Ny + j + 1] - m_domain[i * m_Ny + j - 1]) / (2*m_dy);
    double dPhidz = (m_domain[i * m_Ny + j] - m_domain[i * m_Ny + j - 1]) / (2*m_dz);

    return {-dPhidx, -dPhidy, -dPhidz};  // acceleration vector
}

void ScalarField::doExportCSV(const std::string& filename, const int frame) const {
    std::ofstream fout(filename, std::ios::app); // append mode
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            for (int k = 0; k < m_Nz; ++k) {
                const double z = getZCoord(k);
                fout << frame << "," << x << "," << y << "," << z << "," << m_domain[i * m_Ny * m_Nz + j * m_Nz + k] << "\n";
            }
        }
    }
}