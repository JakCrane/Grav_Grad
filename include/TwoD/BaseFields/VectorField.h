#pragma once
#include "TwoD/BaseFields/BaseField.h"

class VectorField : public BaseField<std::array<double, 2>> {
    public:
        using BaseField<std::array<double, 2>>::BaseField;
        ~VectorField() override = default;

        
        
    protected:
        
        void doExportCSV(const std::string& filename, int frame) const override;

};