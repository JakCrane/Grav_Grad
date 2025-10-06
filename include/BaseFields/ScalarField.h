#pragma once
#include "BaseField.h"

class ScalarField : public BaseField<double> {
    public:
        using BaseField<double>::BaseField;
        ~ScalarField() override = default;

        /**
         * @brief Computes the gradient (negative spatial derivative) at a given (x, y) position.
         */
        std::array<double,2> gradientAt(const double x, const double y) const;

    protected:
        
        void doExportCSV(const std::string& filename, int frame) const override;

};