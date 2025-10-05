#include "Accelerometer.h"

Accelerometer::Accelerometer(std::array<double,2> position, std::array<double,2> orientation)
    : m_position(position), m_orientation(orientation) 
{}

void Accelerometer::update(const GravitationalPotentialField& field) {
    std::array<double,2> gradient = field.gradientAt(m_position[0], m_position[1]);

    m_reading = gradient[0]*m_orientation[0] + gradient[1]*m_orientation[1];
    // Store or process the reading as needed
}

void Accelerometer::exportCSV(const std::string& filename, int frame) const {
    std::ofstream fout(filename, std::ios::app); // append mode
    fout << frame << "," << m_reading << "\n";

}