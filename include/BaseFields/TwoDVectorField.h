#pragma once
#include "BaseField.h"

class TwoDVectorField : public BaseField<std::array<double, 2>> {
    public:
        using BaseField<std::array<double, 2>>::BaseField;
        ~TwoDVectorField() override = default;

        
        
    protected:
        
        void doExportCSV(const std::string& filename, int frame) const override;

};