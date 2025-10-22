#pragma once
#ifdef _OPENMP
#include <omp.h>
#endif
#include "ThreeD/BaseFields/VectorField.h"
#include "ThreeD/MassField.h"
#include "ThreeD/GravitationalPotentialField.h"

template <bool UsePotential>
class GravitationalAccelerationField : public VectorField {
    public:
        GravitationalAccelerationField(int N, double size, const MassField& mass_field, double G);
        ~GravitationalAccelerationField() override = default;

        void update();

    private:

        const MassField* m_mass_field; // current mass field for distributed mass source
        double m_G;
        GravitationalPotentialField m_potential_field;

        std::array<double, 3> computeAcceleration(const double x, const double y, const double z);

};