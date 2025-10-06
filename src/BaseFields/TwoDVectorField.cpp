#include "BaseFields/TwoDVectorField.h"

void TwoDVectorField::doExportCSV(const std::string& filename, const int frame) const {
    std::ofstream fout(filename, std::ios::app); // append mode
    for (int i = 0; i < m_Nx; ++i) {
        const double x = getXCoord(i);
        for (int j = 0; j < m_Ny; ++j) {
            const double y = getYCoord(j);
            fout << frame << "," << x << "," << y << "," << m_domain[i * m_Ny + j][0] << "," << m_domain[i * m_Ny + j][1] << "\n";
        }
    }
}