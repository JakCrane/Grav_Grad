#include <iostream>
#include <array>
#include <cmath>
#include "GravitationalPotentialField.h"
#include "MassField.h"
#include "Accelerometer.h"

int main() {
    const double G = 1.0;
    const double M = 5.0;

    int N = 500;
    double size = 10.0;

    int number_of_frames = 50;
    double radian_change_per_frame = 0.1;

    const char* problem = "square"; // "point" or "square"


    GravitationalPotentialField field(N, size);
    MassField mass_field(N, size);
    Accelerometer accel_0({1.0, 1.0}, {1.0, 0.0});
    Accelerometer accel_1({1.0, 1.0}, {0.0, 1.0});

    // Write CSV header once
    std::ofstream fout_field("potential_field.csv");
    fout_field << "frame,x,y,phi\n";
    fout_field.close();

    std::ofstream fout_square("square_mass_field.csv");
    fout_square << "frame,x,y,phi\n";
    fout_square.close();

    std::ofstream fout_accel_0("accelerometer_0.csv");
    fout_accel_0 << "frame,reading\n";
    fout_accel_0.close();

    std::ofstream fout_accel_1("accelerometer_1.csv");
    fout_accel_1 << "frame,reading\n";
    fout_accel_1.close();


    if (problem == "point") {
        mass_field.setupPoint({0.0, 0.0}, 1.0);
    } else if (problem == "square") {
        mass_field.setupSquare({0.0, 0.0}, 1.0, 1.0);
    } else {
        std::cerr << "Unknown problem type: " << problem << std::endl;
        return 1;
    }

    field.setup(mass_field, G, M);

    // Simulation loop
    for (int frame = 0; frame < number_of_frames; ++frame) {

        std::array<double, 2> mass_pos = {
            2.0 * std::cos(radian_change_per_frame * frame),
            2.0 * std::sin(radian_change_per_frame * frame)
        };

        mass_field.updatePoint(mass_pos);
        field.update(mass_field);
        accel_0.update(field);
        accel_1.update(field);

        field.exportCSV("potential_field.csv", frame);
        mass_field.exportCSV("square_mass_field.csv", frame);
        accel_0.exportCSV("accelerometer_0.csv", frame);
        accel_1.exportCSV("accelerometer_1.csv", frame);

        std::cout << "Frame " << frame << " done" << std::endl;
    }

    std::cout << "Data written to potential_field.csv" << std::endl;
    return 0;
}
