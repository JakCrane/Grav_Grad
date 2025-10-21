#pragma once
#include <array>
#include <vector>
#include <cmath>

#include <fstream>
#include <iostream>

/**
 * @brief Class that holds a 2D field defined on a grid.
 */
template<class T>
class BaseField {
    public:
        // default constructor for empty field
        BaseField() 
            : m_Nx(0), m_Ny(0),
              m_x_min(0.0), m_x_max(0.0), m_y_min(0.0), m_y_max(0.0),
              m_dx(0.0), m_dy(0.0),
              m_domain() {}

        BaseField(int Nx, int Ny, double x_min, double x_max, double y_min, double y_max);
        BaseField(int N, double size);

        virtual ~BaseField() = default;

        /**
         * @brief Gets the x-coordinate of the grid point at index i.
         */
        inline double getYCoord(int j) const { return m_y_min + j * m_dy; }

        /**
         * @brief Gets the x-coordinate of the grid point at index i.
         */
        inline double getXCoord(int i) const { return m_x_min + i * m_dx; }

        /**
         * @brief Returns the value of the field at a given point (x, y).
         * 
         * @param i Index of the point along the x-axis.
         * @param j Index of the point along the y-axis.
         * @return The value of the field at (x, y).
         */
        inline const T& valueAt(int i, int j) const { return m_domain[i * m_Ny + j]; };

        /**
         * @brief Returns a copy of the underlying data.
         */
        std::vector<T> getDomain() const { return m_domain; }

        /**
         * @brief Exports the field data to a CSV file.
         * 
         * @param filename Name of the output CSV file.
         * @param frame Optional frame number for time-dependent fields.
         */
        void exportCSV(const std::string& filename, int frame=0) const {
            doExportCSV(filename, frame); // This pattern allows intellisense to work
        };

        void printInfo() const;

    protected:
        virtual void doExportCSV(const std::string& filename, int frame) const = 0;

        /**
         * @brief Number of Rows
         */
        const int m_Nx;

        /**
         * @brief Number of Columns
         */
        const int m_Ny;

        const double m_x_min, m_x_max, m_y_min, m_y_max;
        const double m_dx, m_dy;

        /**
         * @brief Row major ordered 1D array holding the field values.
         */
        std::vector<T> m_domain;

        template <typename U>
        static constexpr const U& clamp(const U& v, const U& lo, const U& hi) {
            return (v < lo) ? lo : (hi < v) ? hi: v;
        }


};