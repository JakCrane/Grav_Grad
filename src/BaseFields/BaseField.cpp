#include "BaseFields/BaseField.h"

template<class T>
BaseField<T>::BaseField(int Nx, int Ny,
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

template<class T>
BaseField<T>::BaseField(int N, double size)
    : m_Nx(N), m_Ny(N),
      m_x_min(-size / 2), m_x_max(size / 2),
      m_y_min(-size / 2), m_y_max(size / 2),
      m_dx(size / (N - 1)),
      m_dy(size / (N - 1)),
      m_domain(N * N)
{
}

template<class T>
void BaseField<T>::printInfo() const {
    std::cout << "Field Info:\n";
    std::cout << "  Grid size: " << m_Nx << " x " << m_Ny << "\n";
    std::cout << "  X range: [" << m_x_min << ", " << m_x_max << "]\n";
    std::cout << "  Y range: [" << m_y_min << ", " << m_y_max << "]\n";
    std::cout << "  dx: " << m_dx << ", dy: " << m_dy << "\n";
    std::cout << "  Domain values:\n";
    for (int i = 0; i < m_Nx; ++i) {
        for (int j = 0; j < m_Ny; ++j) {
            std::cout << m_domain[i * m_Ny + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

template class BaseField<double>;
template class BaseField<std::array<double,2>>;