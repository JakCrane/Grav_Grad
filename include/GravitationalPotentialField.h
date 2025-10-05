#pragma once
#include "BaseField.h"
#include "MassField.h"


class GravitationalPotentialField : public BaseField {
    public:
        using BaseField::BaseField; // Inherit constructors

        ~GravitationalPotentialField() override = default;

        void setup(const std::array<double, 2> mass_pos, const double G, const double M);
        void setup(const MassField& field, const double G, const double M);
        void update(const std::array<double, 2> mass_pos);
        void update(const MassField& field);

    private:
        // Add member variables if needed
        double m_G, m_M;
        
        Eigen::MatrixXd m_current_mass_field;
        Eigen::MatrixXd m_current_r_inv_field;
        /**
         * @brief Computes the gravitational potential at a given point (x, y) due to a point mass.
         *
         * @param x X-coordinate of the evaluation point.
         * @param y Y-coordinate of the evaluation point.
         * @param mass_pos Position of the point mass as a 2D array [x, y].
         * @param G Gravitational constant.
         * @param M Mass of the point source.
         * @return The computed gravitational potential at (x, y).
        */
        double computePotential(const double x_of_gravitational_potential, const double y_of_gravitational_potential,
                        const std::array<double, 2>& mass_pos);

        // double computePotential(const double x_of_gravitational_potential, const double y_of_gravitational_potential, const MassField& field);
        double computePotential(const double x_of_gravitational_potential, const double y_of_gravitational_potential, const MassField& mass_field);

};