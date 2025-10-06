#pragma once
#include "BaseFields/ScalarField.h"


class MassField : public ScalarField {
    public:
        /** 
         * @brief Constructs a field with a square region of non-zero density centered at initial position.
         */
        MassField(int N, double size, double side_length=1.0, double density=1.0, std::array<double, 2> initial_position={0.0, 0.0});
        ~MassField() override = default;

        /**
         * @brief Updates the position of the square mass region to center around mass_pos.
         */
        void update(std::array<double, 2> mass_pos);

        // prints the domain to console for debugging
        void printDomain() const;

    private:
        double m_density;
        double m_side_length;
        std::array<double, 2> m_current_position;

        void clearField();
        void fillSquareRegion();
};