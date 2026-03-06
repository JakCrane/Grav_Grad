#pragma once
#include <array>
#include <fstream>
#include "Fields/GravitationalAccelerationField.h"

class Accelerometer {
    public:
        Accelerometer(std::array<double,3> position, std::array<double,3> orientation);

        template<bool UsePotential>
        void update(const GravitationalAccelerationField<UsePotential>& field);

        void exportCSV(const std::string& filename, int frame=0) const;

    private:
        std::array<double,3> m_position;
        std::array<double,3> m_orientation;
        
        std::array<double,3> m_acceleration;
        double m_reading;
};

// should remove templates i think theyre wrongly implemented here
template<bool UsePotential>
void Accelerometer::update(const GravitationalAccelerationField<UsePotential>& field) {
    m_acceleration = field.get_acceleration_at(m_position[0], m_position[1], m_position[2]);

    m_reading = m_acceleration[0]*m_orientation[0] + m_acceleration[1]*m_orientation[1] + m_acceleration[2]*m_orientation[2];
}
