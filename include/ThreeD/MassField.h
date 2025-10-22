#pragma once
#include "ThreeD/BaseFields/ScalarField.h"


class MassField : public ScalarField {
    public:
        /** 
         * @brief Constructs a field with a rectangular region of non-zero density centered at initial position.
         * @param N Number of grid points along each axis.
         * @param size Physical size of the domain along each axis.
         * @param side_length_x Length of the sides of the rectangular mass region.
         * @param side_length_y Length of the sides of the rectangular mass region.
         * @param side_length_z Length of the sides of the rectangular mass region.
         * @param density Density value within the rectangular mass region.
         * @param initial_position Initial center position of the rectangular mass region.
         */
        MassField(int N, double size, double side_length_x, double side_length_y, double side_length_z, double density, std::array<double, 3> initial_position={0.0, 0.0, 0.0});

        /** 
         * @brief Constructs a field with a circular region of non-zero density centered at initial position.
         * @param N Number of grid points along each axis.
         * @param size Physical size of the domain along each axis.
         * @param radius Radius of the circular mass region.
         * @param density Density value within the circular mass region.
         * @param initial_position Initial center position of the circular mass region.
         */
        MassField(int N, double size, double radius, double density, std::array<double, 3> initial_position={0.0, 0.0, 0.0});
        ~MassField() override = default;

        /**
         * @brief Updates the position of the square mass region to center around mass_pos.
         */
        void update(std::array<double, 3> mass_pos);


    private:
        double m_density;
        double m_side_length_x;
        double m_side_length_y;
        double m_side_length_z;
        double m_radius;
        enum class FieldType {RECTANGULAR, CIRCULAR};
        FieldType m_type;

        std::array<double, 3> m_current_position;

        void clearField();
        void fillCuboidRegion();
        void fillSphericalRegion();
};