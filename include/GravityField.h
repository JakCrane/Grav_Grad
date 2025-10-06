#pragma once
#ifdef _OPENMP
#include <omp.h>
#endif
#include "BaseFields/TwoDVectorField.h"
#include "MassField.h"


class GravityField : public TwoDVectorField {
    public:
        GravityField(int N, double size, const MassField& mass_field, double G=1.0);

        ~GravityField() override = default;

        void update();

    private:

        const MassField* m_mass_field; // current mass field for distributed mass source
        
        double m_G;

        std::array<double, 2> computeAcceleration(const double x, const double y);

};