#pragma once
#include "BaseFields/ScalarField.h"
#include "MassField.h"

class GravitationalPotentialField : public ScalarField {
    public:
        /** 
         * @brief Default constructor for empty potential field.
         */
        GravitationalPotentialField()
            : ScalarField(), m_mass_field(nullptr), m_G(0.0) {}
            
        /** 
         * @brief Constructs a field of the newtonian potential U = G*sum(M/abs(r-r_ij)).
         * @param N Number of grid points along each axis.
         * @param size Physical size of the domain along each axis.
         * @param mass_field pointer that defines this potential field
         * @param G Gravitational constant.
         */
        GravitationalPotentialField(int N, double size, const MassField* mass_field, double G=1.0);


        ~GravitationalPotentialField() override = default;

        /**
         * @brief Updates the potential based on the current mass distribution.
         */
        void update();


    private:
        const MassField* m_mass_field; // current mass field for distributed mass source
        
        double m_G;

        double computePotential(double x, double y);
};