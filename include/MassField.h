#pragma once
#include "BaseField.h"


class MassField : public BaseField {
    public:
        using BaseField::BaseField; // Inherit constructors

        ~MassField() override = default;

        // sets up square region of non-zero density, centered at initial_position
        void setupSquare(std::array<double, 2> initial_position, double density, double side_length);
        void setupPoint(std::array<double, 2> initial_position, double density);
        // updates position of center of square region
        void updateSquare(std::array<double, 2> mass_pos);
        void updatePoint(std::array<double, 2> mass_pos);
        // prints the domain to console for debugging
        void printDomain() const;

    private:
        double m_density;
        double m_side_length;
        std::array<double, 2> m_current_position;

        void clearField();
        void fillSquareRegion();
        void fillPointRegion();
};