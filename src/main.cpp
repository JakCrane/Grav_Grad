#include <iostream>
#include <array>
#include <cmath>
#ifdef _OPENMP
  #include <omp.h>
#endif

#include "GravityField.h"
#include "MassField.h"
#include "Accelerometer.h"

int main() {
    // // checking for omp
    // #pragma omp parallel
    // {
    //     int tid = 0;
    //     #ifdef _OPENMP
    //     tid = omp_get_thread_num();
    //     #endif
    //     #pragma omp critical
    //     std::cout << "hello from thread " << tid << "\n";
    // }

    const double G = 1.0;
    const double M = 5.0;

    int N = 200;
    double size = 10.0;

    int number_of_frames = 50;

    std::cout << "Starting simulation with " << N << "x" << N << " grid for " << number_of_frames << " frames." << std::endl;
    double radian_change_per_frame = 0.1;

    MassField mass_field(N, size, 0.2, M);
    GravityField grav_field(N, size, mass_field, G);
    // Accelerometer accel_0({1.0, 1.0}, {1.0, 0.0});
    // Accelerometer accel_1({1.0, 1.0}, {0.0, 1.0});

    // Write CSV header once
    std::ofstream fout_field("potential_field.csv");
    fout_field << "frame,x,y,g_x,g_y\n";
    fout_field.close();

    std::ofstream fout_square("square_mass_field.csv");
    fout_square << "frame,x,y,mass\n";
    fout_square.close();

    // std::ofstream fout_accel_0("accelerometer_0.csv");
    // fout_accel_0 << "frame,reading\n";
    // fout_accel_0.close();

    // std::ofstream fout_accel_1("accelerometer_1.csv");
    // fout_accel_1 << "frame,reading\n";
    // fout_accel_1.close();

    // Simulation loop
    for (int frame = 0; frame < number_of_frames; ++frame) {

        std::array<double, 2> mass_pos = {
            2.0 * std::cos(radian_change_per_frame * frame),
            2.0 * std::sin(radian_change_per_frame * frame)
        };

        mass_field.update(mass_pos);
        grav_field.update();
        // accel_0.update(field);
        // accel_1.update(field);

        grav_field.exportCSV("potential_field.csv", frame);
        mass_field.exportCSV("square_mass_field.csv", frame);
        // accel_0.exportCSV("accelerometer_0.csv", frame);
        // accel_1.exportCSV("accelerometer_1.csv", frame);

        std::cout << "Frame " << frame << " done" << std::endl;
    }

    std::cout << "Data written to potential_field.csv" << std::endl;
    return 0;
}
