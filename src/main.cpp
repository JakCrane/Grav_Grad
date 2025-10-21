#include <iostream>
#include <array>
#include <cmath>
#ifdef _OPENMP
  #include <omp.h>
#endif

#include "GravitationalAccelerationField.h"
#include "MassField.h"
#include "Accelerometer.h"

int main() {
    const double G = 1.0;
    const double M = 5.0;

    int N = 500; // number of grid points along each axis
    double size = 4.0; // physical size of the domain along each axis

    int number_of_frames = 3;

    std::cout << "Starting simulation with " << N << "x" << N << " grid for " << number_of_frames << " frames." << std::endl;
    
    MassField mass_field(N, size, 1, M); // circular mass region: radius 0.5, density M
    GravitationalAccelerationField<true> grav_field(N, size, mass_field, G);
    // Accelerometer accel_0({1.0, 1.0}, {1.0, 0.0});
    // Accelerometer accel_1({1.0, 1.0}, {0.0, 1.0});
    
    // Write CSV header once
    std::ofstream fout_field("field_strength.csv");
    fout_field << "frame,x,y,g_x,g_y\n";
    fout_field.close();

    std::ofstream fout_mass("mass_field.csv");
    fout_mass << "frame,x,y,mass\n";
    fout_mass.close();

    // std::ofstream fout_accel_0("accelerometer_0.csv");
    // fout_accel_0 << "frame,reading\n";
    // fout_accel_0.close();
    
    // std::ofstream fout_accel_1("accelerometer_1.csv");
    // fout_accel_1 << "frame,reading\n";
    // fout_accel_1.close();
    
    
    const double translation_per_frame = 0.05;
    // const double radian_change_per_frame = 0.1;

    // Simulation loop

    for (int frame = 0; frame < number_of_frames; ++frame) {

        std::array<double, 2> mass_pos = {
            0,
            translation_per_frame * frame
            // 2.0 * std::cos(radian_change_per_frame * frame),
            // 2.0 * std::sin(radian_change_per_frame * frame)
        };

        mass_field.update(mass_pos);
        grav_field.update();
        // accel_0.update(field);
        // accel_1.update(field);

        grav_field.exportCSV("field_strength.csv", frame);
        mass_field.exportCSV("mass_field.csv", frame);
        // accel_0.exportCSV("accelerometer_0.csv", frame);
        // accel_1.exportCSV("accelerometer_1.csv", frame);

        std::cout << "Frame " << frame << " done" << std::endl;
    }

    std::cout << "Data written to potential_field.csv" << std::endl;
    return 0;
}
