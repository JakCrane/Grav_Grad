#pragma once
#include "ThreeD/BaseFields/BaseField.h"

class VectorField : public BaseField<std::array<double, 3>> {
    public:
        using BaseField<std::array<double, 3>>::BaseField;
        ~VectorField() override = default;

        
        
    protected:
        
        void doExportCSV(const std::string& filename, int frame) const override;

};