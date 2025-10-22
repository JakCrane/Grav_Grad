// #pragma once
// #include <array>
// #include <fstream>
// #include "GravityField.h"

// class Accelerometer {
//     public:
//         Accelerometer(std::array<double,2> position, std::array<double,2> orientation);

//         void update(const GravityField& field);
//         void exportCSV(const std::string& filename, int frame=0) const;

//     private:
//         double m_reading;
//         std::array<double,2> m_position;
//         std::array<double,2> m_orientation;
// };