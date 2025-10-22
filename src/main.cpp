#include <iostream>
#include <array>
#include <cmath>
#ifdef _OPENMP
  #include <omp.h>
#endif

#include "TwoD/GravitationalAccelerationField.h"
#include "TwoD/MassField.h"

int main() {
    const double G = 1.0;
    const double M = 5.0;

    int N = 500; // number of grid points along each axis
    double size = 4.0; // physical size of the domain along each axis

    int number_of_frames = 3;

    std::cout << "Starting simulation with " << N << "x" << N << " grid for " << number_of_frames << " frames." << std::endl;
    
    MassField mass_field(N, size, 1, M); // circular mass region: radius 0.5, density M
    GravitationalAccelerationField<true> grav_field(N, size, mass_field, G);
    
    // Write CSV header once
    std::ofstream fout_field("acceleration_field.csv");
    fout_field << "frame,x,y,g_x,g_y\n";
    fout_field.close();

    std::ofstream fout_mass("mass_field.csv");
    fout_mass << "frame,x,y,mass\n";
    fout_mass.close();  
    
    const double translation_per_frame = 0.05;

    for (int frame = 0; frame < number_of_frames; ++frame) {

        std::array<double, 2> mass_pos = {
            0,
            translation_per_frame * frame
        };

        mass_field.update(mass_pos);
        grav_field.update();

        grav_field.exportCSV("acceleration_field.csv", frame);
        mass_field.exportCSV("mass_field.csv", frame);

        std::cout << "Frame " << frame << " done" << std::endl;
    }

    std::cout << "Data written to potential_field.csv" << std::endl;
    return 0;
}
