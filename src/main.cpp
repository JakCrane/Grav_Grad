#include <iostream>
#include <array>
#include <cmath>
#ifdef _OPENMP
  #include <omp.h>
#endif

#include "ThreeD/GravitationalAccelerationField.h"
#include "ThreeD/MassField.h"

int main() {
    const double G = 1.0;
    const double M = 5.0;

    int N = 100; // number of grid points along each axis
    double size = 4.0; // physical size of the domain along each axis

    int number_of_frames = 20;

    std::cout << "Starting simulation with " << N << "x" << N << " grid for " << number_of_frames << " frames." << std::endl;
    
    MassField mass_field(N, size, 0.5, M); // circular mass region
    GravitationalAccelerationField<true> grav_field(N, size, mass_field, G);

    // Write CSV header once
    std::ofstream fout_field("acceleration_field.csv");
    fout_field << "frame,x,y,z,g_x,g_y,g_z\n";
    fout_field.close();

    std::ofstream fout_mass("mass_field.csv");
    fout_mass << "frame,x,y,z,mass\n";
    fout_mass.close();  
    
    const double translation_per_frame = 0.05;

    for (int frame = 0; frame < number_of_frames; ++frame) {

        std::array<double, 3> mass_pos = {
            0,
            translation_per_frame * frame,
            0
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
