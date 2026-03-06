#include <iostream>
#include <array>
#include <cmath>
#ifdef _OPENMP
  #include <omp.h>
#endif

#include "Fields/GravitationalAccelerationField.h"
#include "Fields/MassField.h"
#include "Accelerometer.h"

int main() {
    const double G = 6.67430e-11;
    const double rho = 19300.0; 
    const double radius_of_mass_cylinder = 0.1;
    const double depth_of_mass_cylinder = 0.03;

    const int N = 80; // number of grid points along each axis
    const double size = 1; // physical size of the domain along each axis

    const int number_of_frames = 40;

    std::cout << "Starting simulation with " << N << "x" << N << " grid for " << number_of_frames << " frames." << std::endl;
    
    MassField mass_field(N, size, radius_of_mass_cylinder, depth_of_mass_cylinder, rho); 
    GravitationalAccelerationField<true> grav_field(N, size, mass_field, G);
    Accelerometer accel({0,0,0},{0,1,0});

    // // Write CSV header once
    // std::ofstream fout_field("logs/acceleration_field.csv");
    // fout_field << "frame,x,y,z,g_x,g_y,g_z\n";
    // fout_field.close();

    // std::ofstream fout_mass("logs/mass_field.csv");
    // fout_mass << "frame,x,y,z,mass\n";
    // fout_mass.close();  

    std::ofstream fout_accel("logs/accelerometer.csv");
    fout_accel << "frame,reading\n";
    fout_accel.close();  
    
    for (int frame = 0; frame < number_of_frames; ++frame) {

        std::array<double, 3> mass_pos = {
            0.025 + depth_of_mass_cylinder/2,
            0.035 * std::sin(2*M_PI/number_of_frames * frame),
            0,
        };

        mass_field.update(mass_pos);
        grav_field.update();
        accel.update(grav_field);

        // grav_field.exportCSV("logs/acceleration_field.csv", frame);
        // mass_field.exportCSV("logs/mass_field.csv", frame);
        accel.exportCSV("logs/accelerometer.csv", frame);

        std::cout << "Frame " << frame << " done" << std::endl;
    }

    std::cout << "Data written to potential_field.csv" << std::endl;
    return 0;
}
