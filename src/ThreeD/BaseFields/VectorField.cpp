#include "ThreeD/BaseFields/VectorField.h"

void VectorField::doExportCSV(const std::string& filename, const int frame) const {
    std::ofstream fout(filename, std::ios::app); // append mode
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            for (int k = 0; k < m_Nz; ++k) {
                const double z = getZCoord(k);
                fout << frame << "," << x << "," << y << "," << z << "," << m_domain[i * m_Ny * m_Nz + j * m_Nz + k][0] << "," << m_domain[i * m_Ny * m_Nz + j * m_Nz + k][1] << "," << m_domain[i * m_Ny * m_Nz + j * m_Nz + k][2] << "\n";
            }
        }
    }
}