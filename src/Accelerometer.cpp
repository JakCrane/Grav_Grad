#include "Accelerometer.h"

Accelerometer::Accelerometer(std::array<double,3> position, std::array<double,3> orientation)
    : m_position(position), m_orientation(orientation) 
{}


void Accelerometer::exportCSV(const std::string& filename, int frame) const {
    std::ofstream fout(filename, std::ios::app); // append mode
    fout << frame << "," << m_reading << "\n";

}